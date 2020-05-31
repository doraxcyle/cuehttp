/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef CUEHTTP_HTTP_PARSER_HPP_
#define CUEHTTP_HTTP_PARSER_HPP_

#include <memory>

#include "cuehttp/context.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/3rd_party/llhttp.h"

namespace cue {
namespace http {
namespace detail {

class http_parser final : safe_noncopyable {
public:
    http_parser(detail::reply_handler handler, bool https, detail::ws_send_handler ws_send_handler) noexcept
        : buffer_(HTTP_REQUEST_BUFFER_SIZE),
          handler_{std::move(handler)},
          https_{https},
          ws_send_handler_{std::move(ws_send_handler)} {
        init_parser();
    }

    std::pair<char*, std::size_t> buffer() noexcept {
        return std::make_pair(buffer_.data() + buffer_offset_, buffer_.size() - buffer_offset_);
    }

    int parse(std::size_t size) noexcept {
        auto code = llhttp_execute(&parser_, buffer_.data() + buffer_offset_, size);
        if (!finished_) {
            code = HPE_USER;
        }
        // code 22 for websocket
        switch (code) {
        case 0:
        case 22: // websocket
            return 0;
        case 21: // pipeline
            return 1;
        case 23: // not complete
            expand();
            return -2;
        default: // error
            break;
        }

        return -1;
    }

    const auto& contexts() const noexcept {
        return contexts_;
    }

    void reset() noexcept {
        if (buffer_.size() != HTTP_REQUEST_BUFFER_SIZE) {
            buffer_.resize(HTTP_REQUEST_BUFFER_SIZE);
        }
        buffer_offset_ = 0;
        finished_ = false;
        contexts_.clear();
    }

private:
    void init_parser() noexcept {
        llhttp_settings_init(&parser_settings_);
        parser_settings_.on_message_begin = &http_parser::on_message_begin;
        parser_settings_.on_url = &http_parser::on_url;
        parser_settings_.on_header_field = &http_parser::on_header_field;
        parser_settings_.on_header_value = &http_parser::on_header_value;
        parser_settings_.on_headers_complete = &http_parser::on_headers_complete;
        parser_settings_.on_body = &http_parser::on_body;
        parser_settings_.on_message_complete = &http_parser::on_message_complete;

        llhttp_init(&parser_, HTTP_REQUEST, &parser_settings_);
        parser_.data = this;
    }

    static int on_message_begin(llhttp_t* parser) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        self->contexts_.emplace_back(new context{self->handler_, self->https_, self->ws_send_handler_});

        return 0;
    }

    static int on_url(llhttp_t* parser, const char* at, std::size_t length) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        auto& request = self->contexts_.back()->req();
        request.url_ = std::string_view{at, length};
        request.parse_url();

        return 0;
    }

    static int on_header_field(llhttp_t* parser, const char* at, std::size_t length) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        self->field_ = std::string_view{at, length};

        return 0;
    }

    static int on_header_value(llhttp_t* parser, const char* at, std::size_t length) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        self->value_ = std::string_view{at, length};
        // add header
        if (!self->field_.empty() && !self->value_.empty()) {
            auto& request = self->contexts_.back()->req();
            request.headers_.emplace(self->field_, self->value_);
        }

        self->field_ = std::string_view{};
        self->value_ = std::string_view{};

        return 0;
    }

    static int on_headers_complete(llhttp_t* parser) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        auto& request = self->contexts_.back()->req();
        // version
        request.major_version_ = parser->http_major;
        request.minor_version_ = parser->http_minor;

        // method
        request.method_ = detail::utils::to_method_string(parser->method);

        // host, hostname, origin, href
        request.host_ = request.get("Host");
        request.hostname_ = request.host_.substr(0, request.host_.rfind(":"));

        // host, hostname, origin, href
        request.host_ = request.get("Host");
        request.hostname_ = request.host_.substr(0, request.host_.rfind(":"));

        // content_type, charset
        std::string_view content_type{request.get("Content-Type")};
        const auto pos = content_type.find("charset");
        if (pos != std::string_view::npos) {
            request.content_type_ = content_type.substr(0, content_type.find(";"));
            request.charset_ = content_type.substr(pos + 8);
        } else {
            request.content_type_ = content_type;
        }

        // content_length
        if (parser->flags & F_CONTENT_LENGTH) {
            request.content_length_ = parser->content_length;
        }

        // keepalive/websocket
        request.keepalive_ = llhttp_should_keep_alive(&self->parser_);
        if (request.keepalive_ && self->parser_.upgrade) {
            const auto upgrade = request.get("Upgrade");
            const auto key = request.get("Sec-WebSocket-Key");
            const auto ws_version = request.get("Sec-WebSocket-Version");
            if (!key.empty() && !ws_version.empty() && detail::utils::iequals(upgrade, "websocket")) {
                request.websocket_ = true;
            }
        }

        // cookie
        const auto cookie_string = request.get("Cookie");
        if (!cookie_string.empty()) {
            request.cookies_.parse(cookie_string);
        }

        return 0;
    }

    static int on_body(llhttp_t* parser, const char* at, std::size_t length) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        auto& request = self->contexts_.back()->req();
        request.body_.append(at, length);

        return 0;
    }

    static int on_message_complete(llhttp_t* parser, const char* at, std::size_t length) {
        http_parser* self{static_cast<http_parser*>(parser->data)};
        self->finished_ = true;

        return 0;
    }

    void expand() {
        const char* data{buffer_.data()};
        buffer_offset_ = buffer_.size();
        buffer_.resize(buffer_.size() * 2);
        for (auto& ctx : contexts_) {
            auto& request = ctx->req();
            std::map<std::string_view, std::string_view> headers;
            for (const auto& header : request.headers_) {
                headers.emplace(
                    std::string_view{buffer_.data() + (header.first.data() - data), header.first.length()},
                    std::string_view{buffer_.data() + (header.second.data() - data), header.second.length()});
            }
            headers.swap(request.headers_);

            if (!request.url_.empty()) {
                request.url_ = std::string_view{buffer_.data() + (request.url_.data() - data), request.url_.length()};
            }

            if (!request.path_.empty()) {
                request.path_ =
                    std::string_view{buffer_.data() + (request.path_.data() - data), request.path_.length()};
            }

            if (!request.querystring_.empty()) {
                request.querystring_ = std::string_view{buffer_.data() + (request.querystring_.data() - data),
                                                        request.querystring_.length()};
            }
        }
    }

    static constexpr std::size_t HTTP_REQUEST_BUFFER_SIZE{2048};
    std::vector<char> buffer_;
    std::size_t buffer_offset_{0};
    detail::reply_handler handler_;
    bool https_{false};
    bool finished_{false};
    detail::ws_send_handler ws_send_handler_;
    llhttp_t parser_;
    llhttp_settings_t parser_settings_;
    std::vector<std::unique_ptr<context>> contexts_;
    std::string_view field_;
    std::string_view value_;
};

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_HTTP_PARSER_HPP_

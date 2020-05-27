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

#ifndef CUEHTTP_REQUEST_HPP_
#define CUEHTTP_REQUEST_HPP_

#include <memory>
#include <vector>

#include "cuehttp/response.hpp"
#include "cuehttp/cookies.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/3rd_party/llhttp.h"

namespace cue {
namespace http {

class request final : safe_noncopyable {
public:
    request(bool https, cookies& cookies) noexcept
        : buffer_(HTTP_REQUEST_BUFFER_SIZE), https_{https}, cookies_{cookies} {
        init_parser();
    }

    unsigned major_version() const noexcept {
        return major_version_;
    }

    unsigned minor_version() const noexcept {
        return minor_version_;
    }

    std::string_view get(std::string_view field) const noexcept {
        for (const auto& header : headers_) {
            if (detail::utils::iequals(header.first, field)) {
                return header.second;
            }
        }
        using namespace std::literals;
        return ""sv;
    }

    const std::map<std::string_view, std::string_view>& headers() const noexcept {
        return headers_;
    }

    std::string_view method() const noexcept {
        return method_;
    }

    std::string_view host() const noexcept {
        return host_;
    }

    std::string_view hostname() const noexcept {
        return hostname_;
    }

    std::string_view url() const noexcept {
        return url_;
    }

    std::string_view origin() const noexcept {
        if (origin_.empty()) {
            origin_ = https_ ? "https://" : "http://" + std::string{host_};
        }
        return origin_;
    }

    std::string_view href() const noexcept {
        if (href_.empty()) {
            href_ = std::string{origin()} + std::string{url_};
        }
        return href_;
    }

    std::string_view path() const noexcept {
        return path_;
    }

    std::string_view querystring() const noexcept {
        return querystring_;
    }

    const std::multimap<std::string, std::string>& query() const noexcept {
        if (!querystring_.empty() && query_.empty()) {
            query_ = detail::utils::parse_query(querystring_);
        }
        return query_;
    }

    std::string_view search() const noexcept {
        if (search_.empty() && !querystring_.empty()) {
            search_ = std::string{"?"} + std::string{querystring_};
        }
        return search_;
    }

    std::string_view type() const noexcept {
        return content_type_;
    }

    std::string_view charset() const noexcept {
        return charset_;
    }

    std::uint64_t length() const noexcept {
        return content_length_;
    }

    bool keepalive() const noexcept {
        return keepalive_;
    }

    std::string_view body() const noexcept {
        return body_;
    }

    std::pair<char*, std::size_t> buffer() noexcept {
        return std::make_pair(buffer_.data() + buffer_offset_, buffer_.size() - buffer_offset_);
    }

    int parse(std::size_t size) noexcept {
        auto code = llhttp_execute(&parser_, buffer_.data() + buffer_offset_, size);
        if (!finished_) {
            code = HPE_USER;
        }
        // code 21 for websocket
        if (code == 0 || code == 21) {
            return 1;
        } else if (code >= 4 && code <= 14) {
            return -1;
        } else {
            buffer_.resize(buffer_.size() * 2);
            return -2;
        }
    }

    bool websocket() const noexcept {
        return websocket_;
    }

    void reset() noexcept {
        if (buffer_.size() != HTTP_REQUEST_BUFFER_SIZE) {
            buffer_.resize(HTTP_REQUEST_BUFFER_SIZE);
        }
        buffer_offset_ = 0;
        finished_ = false;
        origin_.clear();
        href_.clear();
        query_.clear();
        search_.clear();
        content_length_ = 0;
        keepalive_ = false;
        websocket_ = false;
        cookies_.reset();
    }

private:
    void init_parser() noexcept {
        llhttp_settings_init(&parser_settings_);
        parser_settings_.on_message_begin = &request::on_message_begin;
        parser_settings_.on_url = &request::on_url;
        parser_settings_.on_header_field = &request::on_header_field;
        parser_settings_.on_header_value = &request::on_header_value;
        parser_settings_.on_headers_complete = &request::on_headers_complete;
        parser_settings_.on_body = &request::on_body;
        parser_settings_.on_message_complete = &request::on_message_complete;
        // parser_settings_.on_chunk_header = &request::on_chunk_header;
        // parser_settings_.on_chunk_complete = &request::on_chunk_complete;

        llhttp_init(&parser_, HTTP_REQUEST, &parser_settings_);
        parser_.data = this;
    }

    static int on_message_begin(llhttp_t* parser) {
        request* self{static_cast<request*>(parser->data)};
        self->reset();
        return 0;
    }

    static int on_url(llhttp_t* parser, const char* at, std::size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->url_ = std::string_view{at, length};
        self->parse_url();
        return 0;
    }

    static int on_header_field(llhttp_t* parser, const char* at, std::size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->field_ = std::string_view{at, length};
        return 0;
    }

    static int on_header_value(llhttp_t* parser, const char* at, std::size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->value_ = std::string_view{at, length};
        // add header
        if (!self->field_.empty() && !self->value_.empty()) {
            self->headers_.emplace(self->field_, self->value_);
        }
        self->field_ = std::string_view{};
        self->value_ = std::string_view{};
        return 0;
    }

    static int on_headers_complete(llhttp_t* parser) {
        request* self{static_cast<request*>(parser->data)};

        // version
        self->major_version_ = parser->http_major;
        self->minor_version_ = parser->http_minor;

        // method
        self->method_ = detail::utils::to_method_string(parser->method);

        // host, hostname, origin, href
        self->host_ = self->get("Host");
        self->hostname_ = self->host_.substr(0, self->host_.rfind(":"));

        // content_type, charset
        std::string_view content_type{self->get("Content-Type")};
        const auto pos = content_type.find("charset");
        if (pos != std::string_view::npos) {
            self->content_type_ = content_type.substr(0, content_type.find(";"));
            self->charset_ = content_type.substr(pos + 8);
        } else {
            self->content_type_ = content_type;
        }

        // content_length
        if (parser->flags & F_CONTENT_LENGTH) {
            self->content_length_ = parser->content_length;
        }

        // keepalive/websocket
        self->keepalive_ = llhttp_should_keep_alive(&self->parser_);
        if (self->keepalive_ && self->parser_.upgrade) {
            const auto upgrade = self->get("Upgrade");
            const auto key = self->get("Sec-WebSocket-Key");
            const auto ws_version = self->get("Sec-WebSocket-Version");
            if (!key.empty() && !ws_version.empty() && detail::utils::iequals(upgrade, "websocket")) {
                self->websocket_ = true;
            }
        }

        // cookie
        const auto cookie_string = self->get("Cookie");
        if (!cookie_string.empty()) {
            self->cookies_.parse(cookie_string);
        }

        return 0;
    }

    static int on_body(llhttp_t* parser, const char* at, std::size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->body_ = std::string_view{at, length};
        return 0;
    }

    static int on_message_complete(llhttp_t* parser) {
        request* self{static_cast<request*>(parser->data)};
        self->finished_ = true;
        return 0;
    }

    // static int on_chunk_header(llhttp_t* parser) {
    //     request* self{static_cast<request*>(parser->data)};
    //     return 0;
    // }

    // static int on_chunk_complete(llhttp_t* parser) {
    //     request* self{static_cast<request*>(parser->data)};
    //     return 0;
    // }

    void parse_url() {
        http_parser_url url_parser;
        http_parser_url_init(&url_parser);
        if (http_parser_parse_url(url_.data(), url_.length(), 0, &url_parser) == 0) {
            for (unsigned i{0}; i < UF_MAX; ++i) {
                if ((url_parser.field_set & (1 << i)) == 0) {
                    continue;
                }

                const std::string_view temp{url_.data() + url_parser.field_data[i].off, url_parser.field_data[i].len};
                switch (i) {
                case UF_PATH:
                    path_ = temp;
                    continue;
                case UF_QUERY:
                    querystring_ = temp;
                    continue;
                default:
                    continue;
                }
            }
        }
    }

    constexpr static std::size_t HTTP_REQUEST_BUFFER_SIZE{2048};
    std::vector<char> buffer_;
    std::size_t buffer_offset_{0};
    bool https_{false};
    llhttp_t parser_;
    llhttp_settings_t parser_settings_;
    bool finished_{false};
    unsigned major_version_{1};
    unsigned minor_version_{1};
    std::string_view host_;
    std::string_view hostname_;
    std::string_view url_;
    mutable std::string origin_;
    mutable std::string href_;
    std::string_view path_;
    std::string_view querystring_;
    mutable std::multimap<std::string, std::string> query_;
    mutable std::string search_;
    std::string_view method_;
    std::string_view content_type_;
    std::string_view charset_;
    std::uint64_t content_length_{0};
    bool keepalive_{false};
    bool websocket_{false};
    cookies& cookies_;
    std::string_view body_;
    std::string_view field_;
    std::string_view value_;
    std::map<std::string_view, std::string_view> headers_;
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_REQUEST_HPP_

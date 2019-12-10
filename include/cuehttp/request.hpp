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

#include "cuehttp/response.hpp"
#include "cuehttp/cookies.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/3rd_party/http_parser.h"

namespace cue {
namespace http {

constexpr unsigned HTTP_REQUEST_BUFFER_SIZE{2048};

class request final : safe_noncopyable {
public:
    request(bool https, cookies& cookies, response& response) noexcept
        : https_{https}, cookies_{cookies}, response_{response}, parser_{new http_parser} {
        init_parser();
    }

    ~request() noexcept {
        delete parser_;
    }

    const std::string& version() const noexcept {
        return version_;
    }

    const std::string& get(const std::string& field) const {
        for (const auto& header : headers_) {
            if (detail::utils::iequals(header.first, field)) {
                return header.second;
            }
        }
        return detail::global_value::empty_string();
    }

    const std::map<std::string, std::string>& headers() const noexcept {
        return headers_;
    }

    const std::string& method() const noexcept {
        return method_;
    }

    const std::string& host() const noexcept {
        return host_;
    }

    const std::string& hostname() const noexcept {
        return hostname_;
    }

    const std::string& url() const noexcept {
        return url_;
    }

    const std::string& origin() const noexcept {
        return origin_;
    }

    const std::string& href() const noexcept {
        return href_;
    }

    const std::string& path() const noexcept {
        return path_;
    }

    const std::string& querystring() const noexcept {
        return querystring_;
    }

    const std::map<std::string, std::string>& query() const noexcept {
        return query_;
    }

    const std::string& search() const noexcept {
        return search_;
    }

    const std::string& type() const noexcept {
        return content_type_;
    }

    const std::string& charset() const noexcept {
        return charset_;
    }

    long long length() const noexcept {
        return content_length_;
    }

    bool keepalive() const noexcept {
        return keepalive_;
    }

    const std::string& body() const noexcept {
        return body_;
    }

    std::array<char, HTTP_REQUEST_BUFFER_SIZE>& buffer() noexcept {
        return buffer_;
    }

    int parse(size_t size) noexcept {
        return http_parser_execute(parser_, &parser_settings_, buffer_.data(), size);
    }

    bool has_parse_error() const noexcept {
        return parser_->http_errno;
    }

    bool has_more_requests() const noexcept {
        return has_more_requests_;
    }

    void reset() noexcept {
        has_more_requests_ = true;
        version_.clear();
        host_.clear();
        hostname_.clear();
        url_.clear();
        origin_.clear();
        href_.clear();
        path_.clear();
        querystring_.clear();
        query_.clear();
        search_.clear();
        method_.clear();
        content_type_.clear();
        charset_.clear();
        content_length_ = -1;
        keepalive_ = false;
        cookies_.reset();
        body_.clear();
        body_.shrink_to_fit();
        fields_.clear();
        values_.clear();
        headers_.clear();
    }

private:
    void init_parser() noexcept {
        http_parser_init(parser_, HTTP_REQUEST);
        parser_->data = this;

        http_parser_settings_init(&parser_settings_);
        parser_settings_.on_message_begin = &request::on_message_begin;
        parser_settings_.on_url = &request::on_url;
        parser_settings_.on_header_field = &request::on_header_field;
        parser_settings_.on_header_value = &request::on_header_value;
        parser_settings_.on_headers_complete = &request::on_headers_complete;
        parser_settings_.on_body = &request::on_body;
        parser_settings_.on_message_complete = &request::on_message_complete;
        parser_settings_.on_chunk_header = &request::on_chunk_header;
        parser_settings_.on_chunk_complete = &request::on_chunk_complete;
    }

    static int on_message_begin(http_parser* parser) {
        request* self{static_cast<request*>(parser->data)};
        self->reset();
        return 0;
    }

    static int on_url(http_parser* parser, const char* at, size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->url_.append(at, length);
        self->parse_url();
        return 0;
    }

    static int on_header_field(http_parser* parser, const char* at, size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->fields_.emplace_back(at, length);
        return 0;
    }

    static int on_header_value(http_parser* parser, const char* at, size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->values_.emplace_back(at, length);
        return 0;
    }

    static int on_headers_complete(http_parser* parser) {
        request* self{static_cast<request*>(parser->data)};
        assert(self->fields_.size() == self->values_.size());
        // headers
        for (size_t i{0}; i < self->fields_.size(); ++i) {
            self->headers_.emplace(self->fields_.at(i), self->values_.at(i));
        }
        self->fields_.clear();
        self->values_.clear();

        // version
        self->version_ = std::to_string(parser->http_major) + "." + std::to_string(parser->http_minor);

        // method
        self->method_ = detail::utils::to_method_string(parser->method);

        // host, hostname, origin, href
        self->host_ = self->get("Host");
        self->hostname_ = self->host_.substr(0, self->host_.rfind(":"));
        self->origin_ = self->https_ ? "https://" : "http://" + self->host_;
        self->href_ = self->origin_ + self->url_;

        // content_type, charset
        const std::string content_type{self->get("Content-Type")};
        const auto pos = content_type.find("charset");
        if (pos != std::string::npos) {
            self->content_type_ = content_type.substr(0, content_type.find(";"));
            self->charset_ = content_type.substr(pos + 8);
        } else {
            self->content_type_ = std::move(content_type);
        }

        // content_length
        if (parser->flags & F_CONTENTLENGTH) {
            self->content_length_ = parser->content_length;
        }

        // keepalive
        if (self->version_ == "1.1" && detail::utils::iequals(self->get("Connection"), "keep-alive")) {
            self->keepalive_ = true;
        }

        // cookie
        const std::string cookie_string{self->get("Cookie")};
        if (!cookie_string.empty()) {
            self->cookies_.parse(std::move(cookie_string));
        }

        return 0;
    }

    static int on_body(http_parser* parser, const char* at, size_t length) {
        request* self{static_cast<request*>(parser->data)};
        self->body_.append(at, length);
        return 0;
    }

    static int on_message_complete(http_parser* parser) {
        request* self{static_cast<request*>(parser->data)};
        self->has_more_requests_ = false;
        return 0;
    }

    static int on_chunk_header(http_parser* parser) {
        request* self{static_cast<request*>(parser->data)};
        return 0;
    }

    static int on_chunk_complete(http_parser* parser) {
        request* self{static_cast<request*>(parser->data)};
        return 0;
    }

    void parse_url() {
        http_parser_url url_parser;
        http_parser_url_init(&url_parser);
        if (http_parser_parse_url(url_.data(), url_.length(), 0, &url_parser) == 0) {
            for (unsigned i{0}; i < UF_MAX; ++i) {
                if ((url_parser.field_set & (1 << i)) == 0) {
                    continue;
                }

                const std::string temp{url_.data() + url_parser.field_data[i].off, url_parser.field_data[i].len};
                switch (i) {
                case UF_PATH:
                    path_ = std::move(temp);
                    continue;
                case UF_QUERY:
                    querystring_ = std::move(temp);
                    query_ = detail::utils::parse_query(querystring_);
                    search_ = "?" + querystring_;
                    continue;
                default:
                    continue;
                }
            }
        }
    }

    std::array<char, HTTP_REQUEST_BUFFER_SIZE> buffer_;
    bool https_{false};
    response& response_;
    http_parser* parser_;
    http_parser_settings parser_settings_;
    bool has_more_requests_{true};
    std::string version_;
    std::string host_;
    std::string hostname_;
    std::string url_;
    std::string origin_;
    std::string href_;
    std::string path_;
    std::string querystring_;
    std::map<std::string, std::string> query_;
    std::string search_;
    std::string method_;
    std::string content_type_;
    std::string charset_;
    long long content_length_{-1};
    bool keepalive_{false};
    cookies& cookies_;
    std::string body_;
    std::vector<std::string> fields_;
    std::vector<std::string> values_;
    std::map<std::string, std::string> headers_;
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_REQUEST_HPP_

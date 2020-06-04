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
#include "cuehttp/deps/picohttpparser.h"

namespace cue {
namespace http {
namespace detail {

class http_parser final : safe_noncopyable {
public:
    explicit http_parser(context& ctx) noexcept : buffer_(HTTP_REQUEST_BUFFER_SIZE), context_{ctx} {
    }

    std::pair<char*, std::size_t> buffer() noexcept {
        return std::make_pair(buffer_.data() + buffer_offset_, buffer_.size() - buffer_offset_);
    }

    int parse(std::size_t size) noexcept {
        data_size_ += size;
        int code{0};
        if (continue_parse_body_) {
            auto& request = context_.req();
            request.body_ = std::string_view{
                request.body_.data(),
                std::min(request.content_length_, data_size_ - (request.body_.data() - buffer_.data()))};
            if (request.body_.length() < request.content_length_) {
                expand();
                continue_parse_body_ = true;
                code = -2;
            } else {
                continue_parse_body_ = false;
                parse_size_ += request.content_length_;
                if (data_size_ > parse_size_) {
                    buffer_offset_ = parse_size_;
                    code = -3;
                } else {
                    code = 0;
                }
            }
        } else {
            phr_num_headers_ = HTTP_REQUEST_HEADER_SIZE;
            if (size == 0) {
                context_.reset();
            }
            code = phr_parse_request(buffer_.data() + buffer_offset_, buffer_.size() - buffer_offset_, &phr_method_,
                                     &phr_method_len_, &phr_path_, &phr_path_len_, &phr_minor_version_, phr_headers_,
                                     &phr_num_headers_, pre_length_);
            if (code > 0) {
                // method
                parse_size_ += code;
                auto& request = context_.req();
                request.method_ = std::string_view{phr_method_, phr_method_len_};
                // url
                request.url_ = std::string_view{phr_path_, phr_path_len_};
                request.parse_url();
                // headers
                for (std::size_t i{0}; i < phr_num_headers_; ++i) {
                    const auto& header = phr_headers_[i];
                    request.headers_.emplace_back(std::make_pair(std::string_view{header.name, header.name_len},
                                                                 std::string_view{header.value, header.value_len}));
                }

                // content_length
                auto length_value = request.get("content-length");
                if (!length_value.empty()) {
                    request.content_length_ = std::atoll(length_value.data());
                }

                // minor_version
                request.minor_version_ = phr_minor_version_;

                // websocket
                if (phr_minor_version_ && detail::utils::iequals(request.get("connection"), "upgrade")) {
                    const auto upgrade = request.get("upgrade");
                    const auto key = request.get("sec-websocket-key");
                    const auto ws_version = request.get("sec-websocket-version");
                    if (!key.empty() && !ws_version.empty() && detail::utils::iequals(upgrade, "websocket")) {
                        request.websocket_ = true;
                    }
                }

                // cookie
                const auto cookie_string = request.get("cookie");
                if (!cookie_string.empty()) {
                    request.cookies_.parse(cookie_string);
                }

                if (request.content_length_ > 0) {
                    request.body_ =
                        std::string_view{buffer_.data() + code, std::min(request.content_length_, data_size_ - code)};
                    if (request.body_.length() < request.content_length_) {
                        expand();
                        continue_parse_body_ = true;
                        code = -2;
                    } else {
                        parse_size_ += request.content_length_;
                        continue_parse_body_ = false;
                        if (data_size_ > parse_size_) {
                            code = -3;
                        } else {
                            code = 0;
                        }
                    }
                } else {
                    if (data_size_ > parse_size_) {
                        code = -3;
                    } else {
                        code = 0;
                    }
                }
            } else if (code == -2) {
                expand();
            }
        }

        return code;
    }

    void reset() noexcept {
        if (buffer_.size() != HTTP_REQUEST_BUFFER_SIZE) {
            buffer_.resize(HTTP_REQUEST_BUFFER_SIZE);
        }
        data_size_ = 0;
        parse_size_ = 0;
        buffer_offset_ = 0;
        pre_length_ = 0;
        continue_parse_body_ = false;
        context_.reset();
    }

private:
    void expand() {
        const char* data{buffer_.data()};
        pre_length_ = buffer_.size();
        buffer_.resize(buffer_.size() * 2);
        auto& request = context_.req();
        std::vector<std::pair<std::string_view, std::string_view>> headers;
        for (const auto& header : request.headers_) {
            headers.emplace_back(std::make_pair(
                std::string_view{buffer_.data() + (header.first.data() - data), header.first.length()},
                std::string_view{buffer_.data() + (header.second.data() - data), header.second.length()}));
        }
        headers.swap(request.headers_);

        if (!request.method_.empty()) {
            request.method_ =
                std::string_view{buffer_.data() + (request.method_.data() - data), request.method_.length()};
        }

        if (!request.url_.empty()) {
            request.url_ = std::string_view{buffer_.data() + (request.url_.data() - data), request.url_.length()};
        }

        if (!request.path_.empty()) {
            request.path_ = std::string_view{buffer_.data() + (request.path_.data() - data), request.path_.length()};
        }

        if (!request.querystring_.empty()) {
            request.querystring_ =
                std::string_view{buffer_.data() + (request.querystring_.data() - data), request.querystring_.length()};
        }

        if (!request.body_.empty()) {
            request.body_ = std::string_view{buffer_.data() + (request.body_.data() - data), request.body_.length()};
        }
    }

    static constexpr std::size_t HTTP_REQUEST_BUFFER_SIZE{2048};
    static constexpr std::size_t HTTP_REQUEST_HEADER_SIZE{64};
    std::vector<char> buffer_;
    std::size_t data_size_{0};
    std::size_t parse_size_{0};
    std::size_t buffer_offset_{0};
    std::size_t pre_length_{0};
    const char* phr_method_{nullptr};
    const char* phr_path_{nullptr};
    int phr_pret_;
    int phr_minor_version_;
    phr_header phr_headers_[HTTP_REQUEST_HEADER_SIZE];
    std::size_t phr_buflen_{0};
    std::size_t phr_method_len_;
    std::size_t phr_path_len_;
    std::size_t phr_num_headers_;
    bool continue_parse_body_{false};
    context& context_;
    std::string_view field_;
    std::string_view value_;
};

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_HTTP_PARSER_HPP_

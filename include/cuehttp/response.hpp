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

#ifndef CUEHTTP_RESPONSE_HPP_
#define CUEHTTP_RESPONSE_HPP_

#include <memory>

#include "cuehttp/cookies.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/body_stream.hpp"

namespace cue {
namespace http {

class response final : safe_noncopyable {
public:
    response(cookies& cookies, detail::reply_handler handler) noexcept
        : cookies_{cookies}, reply_handler_{std::move(handler)} {
    }

    void minor_version(unsigned version) noexcept {
        minor_version_ = version;
    }

    unsigned status() const noexcept {
        return status_;
    }

    void status(unsigned status) {
        status_ = status;
    }

    bool has(std::string_view field) const noexcept {
        for (auto it = headers_.begin(); it != headers_.end(); ++it) {
            if (detail::utils::iequals(it->first, field)) {
                return true;
            }
        }
        return false;
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

    template <typename Field, typename Value>
    void set(Field&& field, Value&& value) {
        headers_.emplace_back(std::make_pair(std::forward<Field>(field), std::forward<Value>(value)));
    }

    void set(const std::map<std::string, std::string>& headers) {
        headers_.insert(headers_.end(), headers.begin(), headers.end());
    }

    void set(std::map<std::string, std::string>&& headers) {
        headers_.insert(headers_.end(), std::make_move_iterator(headers.begin()),
                        std::make_move_iterator(headers.end()));
    }

    void remove(std::string_view field) noexcept {
        auto erase = headers_.end();
        for (auto it = headers_.begin(); it != headers_.end(); ++it) {
            if (detail::utils::iequals(it->first, field)) {
                erase = it;
                break;
            }
        }
        if (erase != headers_.end()) {
            headers_.erase(erase);
        }
    }

    template <typename Url>
    void redirect(Url&& url) {
        if (status_ == 404) {
            status(302);
        }
        set("Location", std::forward<Url>(url));
    }

    bool keepalive() const noexcept {
        return keepalive_;
    }

    void keepalive(bool keepalive) {
        if (keepalive && minor_version_) {
            keepalive_ = true;
        } else {
            keepalive_ = false;
            set("Connection", "close");
        }
    }

    template <typename ContentType>
    void type(ContentType&& content_type) {
        set("Content-Type", std::forward<ContentType>(content_type));
    }

    std::uint64_t length() const noexcept {
        return content_length_;
    }

    void length(std::uint64_t content_length) noexcept {
        content_length_ = content_length;
    }

    bool has_body() const noexcept {
        return !body_.empty();
    }

    std::string_view dump_body() const noexcept {
        return body_;
    }

    void chunked() noexcept {
        if (!is_chunked_) {
            is_chunked_ = true;
            set("Transfer-Encoding", "chunked");
        }
    }

    template <typename Body>
    void body(Body&& body) {
        body_ = std::forward<Body>(body);
        length(body_.length());
    }

    void body(const char* buffer, std::size_t size) {
        body_.assign(buffer, size);
        length(body_.length());
    }

    std::ostream& body() {
        assert(reply_handler_);
        is_stream_ = true;
        reply_handler_(header_to_string());
        stream_ =
            std::static_pointer_cast<std::ostream>(std::make_shared<detail::body_ostream>(is_chunked_, reply_handler_));
        return *stream_;
    }

    void reset() {
        headers_.clear();
        status_ = 404;
        keepalive_ = true;
        content_length_ = 0;
        body_.clear();
        is_chunked_ = false;
        is_stream_ = false;
        response_str_.clear();
        stream_.reset();
    }

    bool is_stream() const noexcept {
        return is_stream_;
    }

    void to_string(std::string& str) {
        const auto line = detail::utils::get_response_line(minor_version_ * 1000 + status_);
        str.append(line.data(), line.length());
        // headers
        for (const auto& header : headers_) {
            str.append(header.first);
            str.append(": ");
            str.append(header.second);
            str.append("\r\n");
        }

        // cookies
        const auto& cookies = cookies_.get();
        for (const auto& cookie : cookies) {
            if (cookie.valid()) {
                str.append("Set-Cookie: ");
                str.append(cookie.to_string());
                str.append("\r\n");
            }
        }

        if (!is_chunked_) {
            if (content_length_ != 0) {
                str.append("Content-Length: ");
                str.append(std::to_string(content_length_));
                str.append("\r\n");
                append_date(str);
                str.append("\r\n");
                str.append(body_);
            } else {
                str.append("Content-Length: 0\r\n");
                append_date(str);
                str.append("\r\n");
            }
        } else {
            // chunked
            str.append("\r\n");
        }
    }

private:
    std::string header_to_string() {
        std::ostringstream os;
        os << detail::utils::get_response_line(minor_version_ * 1000 + status_);
        // headers
        // os << detail::utils::to_gmt_date_string(std::time(nullptr));
        for (const auto& header : headers_) {
            os << header.first << ": " << header.second << "\r\n";
        }

        if (get("connection").empty() && keepalive_) {
            os << "Connection: keep-alive\r\n";
        }

        // cookies
        const auto& cookies = cookies_.get();
        for (const auto& cookie : cookies) {
            if (cookie.valid()) {
                os << "Set-Cookie: " << cookie.to_string() << "\r\n";
            }
        }

        if (is_chunked_) {
            os << "\r\n";
        } else {
            os << "Content-Length: " << content_length_ << "\r\n\r\n";
        }

        return os.str();
    }

    void append_date(std::string& str) {
        const auto now = std::chrono::steady_clock::now();
        if (now - last_time_ > std::chrono::seconds{1}) {
            last_gmt_date_str_ = detail::utils::to_gmt_date_string(std::time(nullptr));
            last_time_ = now;
        }
        str.append(last_gmt_date_str_);
    }

    std::vector<std::pair<std::string, std::string>> headers_;
    unsigned minor_version_{1};
    unsigned status_{404};
    bool keepalive_{true};
    std::uint64_t content_length_{0};
    cookies& cookies_;
    std::string body_;
    std::string response_str_;
    bool is_chunked_{false};
    bool is_stream_{false};
    std::chrono::steady_clock::time_point last_time_{std::chrono::steady_clock::now()};
    std::string last_gmt_date_str_;
    detail::reply_handler reply_handler_;
    std::shared_ptr<std::ostream> stream_{nullptr};
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_RESPONSE_HPP_

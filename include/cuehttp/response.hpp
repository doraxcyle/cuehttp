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

    void version_major(unsigned version) {
        version_major_ = version;
    }

    void version_minor(unsigned version) {
        version_minor_ = version;
    }

    unsigned status() const noexcept {
        return status_;
    }

    void status(unsigned status) noexcept {
        status_ = status;
        message_ = detail::utils::get_message_for_status(status);
    }

    template <typename Msg>
    void message(Msg&& message) {
        message_ = std::forward<Msg>(message);
    }

    bool has(const std::string& field) const {
        for (auto it = headers_.begin(); it != headers_.end(); ++it) {
            if (detail::utils::iequals(it->first, field)) {
                return true;
            }
        }
        return false;
    }

    const std::string& get(const std::string& field) const {
        for (const auto& header : headers_) {
            if (detail::utils::iequals(header.first, field)) {
                return header.second;
            }
        }
        return detail::global_value::empty_string();
    }

    template <typename Field, typename Value>
    void set(Field&& field, Value&& value) {
        headers_.emplace(std::forward<Field>(field), std::forward<Value>(value));
    }

    void set(const std::map<std::string, std::string>& headers) {
        headers_.insert(headers.begin(), headers.end());
    }

    void set(std::map<std::string, std::string>&& headers) {
        headers_.insert(std::make_move_iterator(headers.begin()), std::make_move_iterator(headers.end()));
    }

    void remove(const std::string& field) {
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
        if (keepalive && version_major_ == 1) {
            keepalive_ = true;
        } else {
            keepalive_ = false;
            set("Connection", "close");
        }
    }

    template <typename ContentType>
    void type(ContentType&& content_type) {
        set("Content-type", std::forward<ContentType>(content_type));
    }

    void length(long long content_length) noexcept {
        content_length_ = content_length;
    }

    bool has_body() const noexcept {
        return !body_.empty();
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
        valid_ = false;
        reply_handler_(header_to_string());
        stream_ =
            std::static_pointer_cast<std::ostream>(std::make_shared<detail::body_ostream>(chunked(), reply_handler_));
        return *stream_;
    }

    void reset() noexcept {
        headers_.clear();
        status_ = 404;
        message_ = "Not Found";
        keepalive_ = false;
        content_length_ = 0;
        cookies_.reset();
        body_.clear();
        valid_ = true;
        stream_.reset();
    }

    bool valid() const noexcept {
        return valid_;
    }

private:
    friend std::ostream& operator<<(std::ostream& os, const response& response);

    bool chunked() const {
        return detail::utils::iequals(get("Transfer-Encoding"), "chunked");
    }

    std::string header_to_string() {
        std::ostringstream os;
        os << "HTTP/" << version_major_ << '.' << version_minor_ << ' ' << status_ << ' ' << message_ << "\r\n";

        // headers
        os << "Server: cuehttp\r\n";
        for (const auto& header : headers_) {
            if (chunked() && detail::utils::iequals(header.first, "Content-length")) {
                continue;
            }
            os << header.first << ": " << header.second << "\r\n";
        }

        if (get("Connection").empty() && keepalive_) {
            os << "Connection: keep-alive\r\n";
        }

        // cookies
        const auto& cookies = cookies_.get();
        for (const auto& cookie : cookies) {
            if (cookie.valid()) {
                os << "Set-Cookie: " << cookie.to_string() << "\r\n";
            }
        }

        if (chunked()) {
            os << "\r\n";
        } else {
            if (!has("Content-length")) {
                os << "Content-length: " << content_length_ << "\r\n\r\n";
            }
        }
        return os.str();
    }

    std::map<std::string, std::string> headers_;
    unsigned version_major_{1};
    unsigned version_minor_{1};
    unsigned status_{404};
    std::string message_{"Not Found"};
    bool keepalive_{false};
    long long content_length_{0};
    cookies& cookies_;
    std::string body_;
    bool valid_{true};
    detail::reply_handler reply_handler_;
    std::shared_ptr<std::ostream> stream_{nullptr};
};

inline std::ostream& operator<<(std::ostream& os, const response& response) {
    os << "HTTP/" << response.version_major_ << '.' << response.version_minor_ << ' ' << response.status_ << ' '
       << response.message_ << "\r\n";

    // headers
    os << "Server: cuehttp\r\n";
    for (const auto& header : response.headers_) {
        os << header.first << ": " << header.second << "\r\n";
    }

    if (response.get("Connection").empty() && response.keepalive_) {
        os << "Connection: keep-alive\r\n";
    }

    // cookies
    const auto& cookies = response.cookies_.get();
    for (const auto& cookie : cookies) {
        if (cookie.valid()) {
            os << "Set-Cookie: " << cookie.to_string() << "\r\n";
        }
    }

    if (response.chunked()) {
        // chunked
        os << "\r\n";
    } else {
        os << "Content-length: " << response.content_length_ << "\r\n\r\n";
        os << response.body_;
    }
    return os;
}

} // namespace http
} // namespace cue

#endif // CUEHTTP_RESPONSE_HPP_

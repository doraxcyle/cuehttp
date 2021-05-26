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
#include "cuehttp/deps/fmt/fmt.h"
#include "cuehttp/detail/body_stream.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {

using namespace std::literals;

class response final : safe_noncopyable {
 public:
  response(cookies& cookies, detail::reply_handler handler) noexcept
      : cookies_{cookies},
        last_gmt_date_str_{detail::utils::to_gmt_date_string(std::time(nullptr))},
        reply_handler_{std::move(handler)} {}

  void minor_version(unsigned version) noexcept { minor_version_ = version; }

  unsigned status() const noexcept { return status_; }

  void status(unsigned status) { status_ = status; }

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

  template <typename _Field, typename _Value>
  void set(_Field&& field, _Value&& value) {
    headers_.emplace_back(std::make_pair(std::forward<_Field>(field), std::forward<_Value>(value)));
  }

  void set(const std::map<std::string, std::string>& headers) {
    headers_.insert(headers_.end(), headers.begin(), headers.end());
  }

  void set(std::map<std::string, std::string>&& headers) {
    headers_.insert(headers_.end(), std::make_move_iterator(headers.begin()), std::make_move_iterator(headers.end()));
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

  template <typename _Url>
  void redirect(_Url&& url) {
    if (status_ == 404) {
      status(302);
    }
    set("Location", std::forward<_Url>(url));
  }

  bool keepalive() const noexcept { return keepalive_; }

  void keepalive(bool keepalive) {
    if (keepalive && minor_version_) {
      keepalive_ = true;
    } else {
      keepalive_ = false;
      set("Connection", "close");
    }
  }

  template <typename _ContentType>
  void type(_ContentType&& content_type) {
    set("Content-Type", std::forward<_ContentType>(content_type));
  }

  std::uint64_t length() const noexcept { return content_length_; }

  void length(std::uint64_t content_length) noexcept { content_length_ = content_length; }

  bool has_body() const noexcept { return !body_.empty(); }

  std::string_view dump_body() const noexcept { return body_; }

  void chunked() noexcept {
    if (!is_chunked_) {
      is_chunked_ = true;
      set("Transfer-Encoding", "chunked");
    }
  }

  template <typename _Body>
  void body(_Body&& body) {
    body_ = std::forward<_Body>(body);
    length(body_.length());
  }

  void body(const char* buffer, std::size_t size) {
    body_.assign(buffer, size);
    length(body_.length());
  }

  std::ostream& body() {
    assert(reply_handler_);
    if (!stream_) {
      is_stream_ = true;
      reply_handler_(header_to_string());
      stream_ = std::make_shared<detail::body_ostream>(is_chunked_, reply_handler_);
    }
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

  bool is_stream() const noexcept { return is_stream_; }

  void to_string(std::string& str) {
    str += detail::utils::get_response_line(minor_version_ * 1000 + status_);
    // headers
    const auto now = std::chrono::steady_clock::now();
    if (now - last_time_ > std::chrono::seconds{1}) {
      last_gmt_date_str_ = detail::utils::to_gmt_date_string(std::time(nullptr));
      last_time_ = now;
    }
    str += last_gmt_date_str_;
    for (const auto& header : headers_) {
      str += fmt::format("{}: {}\r\n", header.first, header.second);
    }

    // cookies
    const auto& cookies = cookies_.get();
    for (const auto& cookie : cookies) {
      if (cookie.valid()) {
        str += fmt::format("Set-Cookie: {}\r\n", cookie.to_string());
      }
    }

    if (!is_chunked_) {
      if (content_length_ != 0) {
        str += fmt::format("Content-Length: {}\r\n\r\n", content_length_);
        str += body_;
      } else {
        str.append("Content-Length: 0\r\n\r\n");
      }
    } else {
      // chunked
      str.append("\r\n");
    }
  }

 private:
  std::string header_to_string() {
    std::string str{detail::utils::get_response_line(minor_version_ * 1000 + status_)};
    // headers
    // os << detail::utils::to_gmt_date_string(std::time(nullptr));
    for (const auto& header : headers_) {
      str += fmt::format("{}: {}\r\n", header.first, header.second);
    }

    if (get("connection").empty() && keepalive_) {
      str += "Connection: keep-alive\r\n"sv;
    }

    // cookies
    const auto& cookies = cookies_.get();
    for (const auto& cookie : cookies) {
      if (cookie.valid()) {
        str += fmt::format("Set-Cookie: {}\r\n", cookie.to_string());
      }
    }

    if (is_chunked_) {
      str += "\r\n"sv;
    } else {
      str += fmt::format("Content-Length: {}\r\n\r\n", content_length_);
    }

    return str;
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

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_RESPONSE_HPP_

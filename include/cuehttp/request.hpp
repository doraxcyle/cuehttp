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

#include <algorithm>
#include <memory>
#include <vector>

#include "cuehttp/cookies.hpp"
#include "cuehttp/deps/picohttpparser.h"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/response.hpp"

namespace cue {
namespace http {

class request final : safe_noncopyable {
 public:
  request(bool https, response& res, cookies& cookies) noexcept
      : https_{https}, buffer_(HTTP_REQUEST_BUFFER_SIZE), res_{res}, cookies_{cookies} {}

  unsigned minor_version() const noexcept { return minor_version_; }

  std::string_view get(std::string_view field) const noexcept {
    for (std::size_t i{0}; i < phr_num_headers_; ++i) {
      const auto& header = phr_headers_[i];
      if (detail::utils::iequals({header.name, header.name_len}, field)) {
        return {header.value, header.value_len};
      }
    }

    using namespace std::literals;
    return ""sv;
  }

  std::vector<std::pair<std::string_view, std::string_view>> headers() const noexcept {
    std::vector<std::pair<std::string_view, std::string_view>> headers;
    for (std::size_t i{0}; i < phr_num_headers_; ++i) {
      const auto& header = phr_headers_[i];
      headers.emplace_back(std::string_view{header.name, header.name_len},
                           std::string_view{header.value, header.value_len});
    }
    return headers;
  }

  std::string_view method() const noexcept { return method_; }

  std::string_view host() const noexcept { return get("Host"); }

  std::string_view hostname() const noexcept {
    const auto host_str = host();
    return host_str.substr(0, host_str.rfind(":"));
  }

  std::string_view url() const noexcept { return url_; }

  std::string_view origin() const noexcept {
    if (origin_.empty()) {
      origin_ = https_ ? "https://" : "http://";
      origin_ += host();
    }
    return origin_;
  }

  std::string_view href() const noexcept {
    if (href_.empty()) {
      href_ += origin();
      href_ += url_;
    }
    return href_;
  }

  std::string_view path() const noexcept { return path_; }

  std::string_view querystring() const noexcept { return querystring_; }

  const std::multimap<std::string, std::string>& query() const noexcept {
    if (!querystring_.empty() && query_.empty()) {
      query_ = detail::utils::parse_query(querystring_);
    }
    return query_;
  }

  std::string_view search() const noexcept { return search_; }

  std::string_view type() const noexcept {
    const auto content_type = get("Content-Type");
    const auto pos = content_type.find("charset");
    if (pos != std::string_view::npos) {
      return content_type.substr(0, content_type.find(";"));
    } else {
      return content_type;
    }
  }

  std::string_view charset() const noexcept {
    const auto content_type = get("Content-Type");
    const auto pos = content_type.find("charset");
    if (pos != std::string_view::npos) {
      return content_type.substr(pos + 8);
    } else {
      return content_type;
    }
  }

  std::uint64_t length() const noexcept { return content_length_; }

  bool websocket() const noexcept { return websocket_; }

  std::string_view body() const noexcept { return body_; }

  void reset() noexcept {
    data_size_ = 0;
    parse_size_ = 0;
    buffer_offset_ = 0;
    continue_parse_body_ = false;
    field_ = {};
    value_ = {};
    url_ = {};
    origin_.clear();
    href_.clear();
    path_ = {};
    querystring_ = {};
    query_.clear();
    search_ = {};
    method_ = {};
    content_length_ = 0;
    websocket_ = false;
    res_.reset();
    cookies_.reset();
    body_ = {};
  }

  std::pair<char*, std::size_t> buffer() noexcept {
    return {buffer_.data() + buffer_offset_, buffer_.size() - buffer_offset_};
  }

  int parse(std::size_t size) noexcept {
    data_size_ += size;
    int code{0};
    if (continue_parse_body_) {
      body_ = {body_.data(),
               std::min(content_length_, static_cast<std::uint64_t>(data_size_ - (body_.data() - buffer_.data())))};
      if (body_.length() < content_length_) {
        expand();
        continue_parse_body_ = true;
        code = -2;
      } else {
        continue_parse_body_ = false;
        parse_size_ += content_length_;
        if (data_size_ > parse_size_) {
          code = -3;
        } else {
          code = 0;
        }
      }
    } else {
      phr_num_headers_ = HTTP_REQUEST_HEADER_SIZE;
      if (size == 0) {
        res_.reset();
      }
      code = phr_parse_request(buffer_.data(), data_size_, &phr_method_, &phr_method_len_, &phr_path_, &phr_path_len_,
                               &phr_minor_version_, phr_headers_, &phr_num_headers_, parse_size_);
      if (code > 0) {
        // method
        parse_size_ += code;
        method_ = {phr_method_, phr_method_len_};
        // url
        url_ = {phr_path_, phr_path_len_};
        parse_url();

        // content_length
        const auto length_value = get("content-length");
        if (!length_value.empty()) {
          content_length_ = std::atoll(length_value.data());
        }

        // minor_version
        minor_version_ = phr_minor_version_;

        // websocket
        const auto upgrade = get("upgrade");
        if (!upgrade.empty()) {
          if (detail::utils::iequals(upgrade, "websocket")) {
            auto connection_value = get("connection");
            const auto key = get("sec-websocket-key");
            const auto ws_version = get("sec-websocket-version");
            if (!key.empty() && !ws_version.empty() && detail::utils::iequals(connection_value, "upgrade")) {
              websocket_ = true;
            }
          }
        }

        // cookie
        const auto cookie_string = get("cookie");
        if (!cookie_string.empty()) {
          cookies_.parse(cookie_string);
        }

        if (content_length_ > 0) {
          body_ = {buffer_.data() + code, std::min(content_length_, static_cast<std::uint64_t>(data_size_ - code))};
          if (body_.length() < content_length_) {
            expand();
            continue_parse_body_ = true;
            code = -2;
          } else {
            parse_size_ += content_length_;
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
          } else if (data_size_ == parse_size_) {
            code = 0;
          } else {
            expand();
            code = -2;
          }
        }
      } else if (code == -2) {
        expand();
      }
    }

    return code;
  }

 private:
  void parse_url() {
    const auto pos = url_.find('?');
    if (pos == std::string_view::npos) {
      path_ = url_;
    } else {
      path_ = url_.substr(0, pos);
      querystring_ = url_.substr(pos + 1, url_.length() - pos - 1);
      search_ = url_.substr(pos, url_.length() - pos);
    }
  }

  void expand() {
    const char* data{buffer_.data()};
    buffer_offset_ = buffer_.size();
    buffer_.resize(buffer_.size() * 2);

    for (std::size_t i{0}; i < phr_num_headers_; ++i) {
      auto& header = phr_headers_[i];
      header.name = buffer_.data() + (header.name - data);
      header.value = buffer_.data() + (header.value - data);
    }

    if (!method_.empty()) {
      method_ = {buffer_.data() + (method_.data() - data), method_.length()};
    }

    if (!url_.empty()) {
      url_ = {buffer_.data() + (url_.data() - data), url_.length()};
    }

    if (!path_.empty()) {
      path_ = {buffer_.data() + (path_.data() - data), path_.length()};
    }

    if (!querystring_.empty()) {
      querystring_ = {buffer_.data() + (querystring_.data() - data), querystring_.length()};
    }

    if (!body_.empty()) {
      body_ = {buffer_.data() + (body_.data() - data), body_.length()};
    }
  }

  static constexpr std::size_t HTTP_REQUEST_BUFFER_SIZE{4096};
  static constexpr std::size_t HTTP_REQUEST_HEADER_SIZE{64};
  bool https_{false};
  std::vector<char> buffer_;
  std::size_t data_size_{0};
  std::size_t parse_size_{0};
  std::size_t buffer_offset_{0};
  const char* phr_method_{nullptr};
  const char* phr_path_{nullptr};
  int phr_minor_version_;
  phr_header phr_headers_[HTTP_REQUEST_HEADER_SIZE];
  std::size_t phr_method_len_;
  std::size_t phr_path_len_;
  std::size_t phr_num_headers_;
  bool continue_parse_body_{false};
  std::string_view field_;
  std::string_view value_;
  unsigned minor_version_{1};
  std::string_view url_;
  mutable std::string origin_;
  mutable std::string href_;
  std::string_view path_;
  std::string_view querystring_;
  mutable std::multimap<std::string, std::string> query_;
  std::string_view search_;
  std::string_view method_;
  std::uint64_t content_length_{0};
  bool websocket_{false};
  response& res_;
  cookies& cookies_;
  std::string_view body_;
};

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_REQUEST_HPP_

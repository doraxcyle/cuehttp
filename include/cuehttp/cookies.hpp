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

#ifndef CUEHTTP_COOKIES_HPP_
#define CUEHTTP_COOKIES_HPP_

#include <ostream>
#include <string>
#include <unordered_set>

#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {

// RFC6265
class cookie final : safe_noncopyable {
 public:
  struct options final {
    int max_age{-1};
    std::string expires;
    std::string path;
    std::string domain;
    bool secure{false};
    bool http_only{false};

    void reset() noexcept {
      max_age = -1;
      expires.clear();
      path.clear();
      domain.clear();
      secure = false;
      http_only = false;
    }
  };

  cookie() noexcept = default;

  explicit cookie(std::string_view cookie_string) noexcept { parse_cookie(cookie_string); }

  cookie(cookie&& rhs) noexcept {
    name_ = std::move(rhs.name_);
    value_ = std::move(rhs.value_);
    options_ = std::move(rhs.options_);
  }

  cookie& operator=(cookie&& rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }

    name_ = std::move(rhs.name_);
    value_ = std::move(rhs.value_);
    options_ = std::move(rhs.options_);
    return *this;
  }

  std::string_view get(std::string_view name) const noexcept {
    using namespace std::literals;
    return name == name_ ? value_ : ""sv;
  }

  template <typename... _Args>
  void set(_Args&&... args) {
    set_impl(std::forward<_Args>(args)...);
  }

  std::string_view name() const noexcept { return name_; }

  template <typename _Name>
  void name(_Name&& name) {
    name_ = std::forward<_Name>(name);
  }

  std::string_view value() const noexcept { return value_; }

  template <typename _Value>
  void value(_Value&& value) {
    value_ = std::forward<_Value>(value);
  }

  int max_age() const noexcept { return options_.max_age; }

  void max_age(int max_age) noexcept { options_.max_age = max_age; }

  std::string_view expires() const noexcept { return options_.expires; }

  template <typename _Expires>
  void expires(_Expires&& expires) {
    options_.expires = std::forward<_Expires>(expires);
  }

  std::string_view path() const noexcept { return options_.path; }

  template <typename _Path>
  void path(_Path&& path) {
    options_.path = std::forward<_Path>(path);
  }

  std::string_view domain() const noexcept { return options_.domain; }

  template <typename _Domain>
  void domain(_Domain&& domain) {
    options_.domain = std::forward<_Domain>(domain);
  }

  bool secure() const noexcept { return options_.secure; }

  void secure(bool secure) noexcept { options_.secure = secure; }

  bool http_only() const noexcept { return options_.http_only; }

  void http_only(bool http_only) noexcept { options_.http_only = http_only; }

  std::string to_string() const {
    std::ostringstream os;
    os << *this;
    return os.str();
  }

  void parse(std::string_view cookie_string) { parse_cookie(cookie_string); }

  bool valid() const noexcept { return !name_.empty() && !value_.empty(); }

  void reset() noexcept {
    name_.clear();
    value_.clear();
    options_.reset();
  }

 private:
  friend std::ostream& operator<<(std::ostream& os, const cookie& cookie);

  template <typename _Name, typename _Value>
  void set_impl(_Name&& name, _Value&& value) {
    name_ = std::forward<_Name>(name);
    value_ = std::forward<_Value>(value);
  }

  template <typename _Name, typename _Value, typename _Options>
  void set_impl(_Name&& name, _Value&& value, _Options&& options) {
    name_ = std::forward<_Name>(name);
    value_ = std::forward<_Value>(value);
    options_ = std::forward<_Options>(options);
  }

  void parse_cookie(std::string_view cookie_string) {
    assert(!cookie_string.empty());
    using namespace std::literals;
    static const std::unordered_set<std::string_view> options_names{"path"sv, "domain"sv, "max-age"sv, "expires"sv};
    const auto cookie_map = detail::utils::split(cookie_string, "; ");
    for (const auto& cookie : cookie_map) {
      const auto key_value = detail::utils::split(cookie, "=");
      // key=value
      if (key_value.size() >= 2) {
        if (options_names.count(detail::utils::to_lower(key_value[0])) == 0) {
          name_ = std::string{key_value[0]};
          value_ = std::string{key_value[1]};
        } else {
          if (detail::utils::iequals(key_value[0], "path")) {
            options_.path = std::string{key_value[1]};
            continue;
          }
          if (detail::utils::iequals(key_value[0], "domain")) {
            options_.domain = std::string{key_value[1]};
            continue;
          }
          if (detail::utils::iequals(key_value[0], "expires")) {
            options_.expires = std::string{key_value[1]};
            continue;
          }
          if (detail::utils::iequals(key_value[0], "max-age")) {
            options_.max_age = std::atoi(key_value[1].data());
          }
        }
      } else {
        if (detail::utils::iequals(key_value[0], "secure")) {
          options_.secure = true;
          continue;
        }
        if (detail::utils::iequals(key_value[0], "HttpOnly")) {
          options_.http_only = true;
        }
      }
    }
  }

  std::string name_;
  std::string value_;
  options options_;
};

inline std::ostream& operator<<(std::ostream& os, const cookie& cookie) {
  assert(!cookie.name_.empty() && !cookie.value_.empty());
  // name=value
  os << cookie.name_ << '=' << cookie.value_;
  // path
  if (!cookie.options_.path.empty()) {
    os << "; path=" << cookie.options_.path;
  }
  // domain
  if (!cookie.options_.domain.empty()) {
    os << "; domain=" << cookie.options_.domain;
  }
  // max_age
  if (cookie.options_.max_age != -1) {
    os << "; Max-Age=" << cookie.options_.max_age;
    if (cookie.options_.expires.empty()) {
      os << "; expires=" << detail::utils::to_gmt_string(std::time(nullptr) + cookie.options_.max_age);
    }
  }
  // expires
  if (!cookie.options_.expires.empty()) {
    os << "; expires=" << cookie.options_.expires;
  }
  // secure
  if (cookie.options_.secure) {
    os << "; secure";
  }
  // http_only
  if (cookie.options_.http_only) {
    os << "; HttpOnly";
  }
  return os;
}

class cookies final : safe_noncopyable {
 public:
  cookies() noexcept = default;

  std::string_view get(std::string_view name) const noexcept { return cookie_.get(name); }

  template <typename... _Args>
  void set(_Args&&... args) {
    cookie temp;
    temp.set(std::forward<_Args>(args)...);
    cookies_.emplace_back(std::move(temp));
  }

  void parse(std::string_view cookie_string) { cookie_.parse(cookie_string); }

  const std::vector<cookie>& get() const noexcept { return cookies_; }

  void reset() noexcept { cookies_.clear(); }

 private:
  cookie cookie_;
  std::vector<cookie> cookies_;
};

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_COOKIES_HPP_

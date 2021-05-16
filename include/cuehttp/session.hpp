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

#ifndef CUEHTTP_SESSION_HPP_
#define CUEHTTP_SESSION_HPP_

#include <functional>
#include <memory>

#include "cuehttp/cookies.hpp"
#include "cuehttp/deps/json.hpp"

namespace cue {
namespace http {

class context;

class session final {
 public:
  struct store_t final {
    std::function<std::string(std::string_view)> get;
    std::function<void(std::string_view, std::string_view, std::uint32_t)> set;
    std::function<void(std::string_view)> destroy;

    explicit operator bool() const noexcept { return get && set && destroy; }
  };

  struct external_key_t final {
    std::function<std::string(context&)> get;
    std::function<void(context&, std::string_view)> set;
    std::function<void(context&, std::string_view)> destroy;

    explicit operator bool() const noexcept { return get && set && destroy; }
  };

  struct options final {
    std::string key{"cuehttp"};
    // default one day seconds
    int max_age{24 * 60 * 60};
    bool auto_commit{true};
    store_t store;
    external_key_t external_key;
    std::function<std::string()> genid;
    std::string prefix;
  };

  session() = delete;

  template <typename _Options>
  session(_Options&& options, context& context, cookies& cookies) noexcept
      : options_{std::forward<_Options>(options)}, context_{context}, cookies_{cookies} {
    if (!options_.genid) {
      options_.genid = [this]() { return options_.prefix + detail::utils::uuid(); };
    }

    if (options_.store) {
      init_from_external();
    } else {
      init_from_cookie();
    }
  }

  template <typename _Key, typename _Value>
  void set(_Key&& key, _Value&& value) {
    datas_[std::forward<_Key>(key)] = std::forward<_Value>(value);
  }

  std::string_view get(const std::string& key) const noexcept {
    const auto it = datas_.find(key);
    if (it != datas_.end()) {
      return it->second;
    }
    using namespace std::literals;
    return ""sv;
  }

  void remove() noexcept { remove_ = true; }

  void commit() {
    if (remove_) {
      destroy();
      return;
    }

    if (datas_.empty()) {
      return;
    }

    if (pre_json_ == to_string()) {
      return;
    }

    auto json = to_json();
    if (options_.max_age == -1) {
      json["_session"] = true;
    } else {
      json["_max_age"] = options_.max_age;
      json["_expire"] = options_.max_age + detail::utils::now();
    }

    if (options_.store) {
      options_.store.set(external_key_, json.dump(), options_.max_age);
      if (options_.external_key) {
        options_.external_key.set(context_, external_key_);
      } else {
        cookie::options options;
        options.max_age = options_.max_age;
        cookies_.set(options_.key, external_key_, std::move(options));
      }
      return;
    }
    const auto value = detail::utils::base64_encode(json.dump());
    cookie::options options;
    options.max_age = options_.max_age;
    cookies_.set(options_.key, std::move(value), std::move(options));
  }

 private:
  void init_from_cookie() {
    const auto cookie = cookies_.get(options_.key);
    const auto json = detail::utils::base64_decode(cookie);
    if (!json.empty() && parse(json)) {
      pre_json_ = to_string();
    }
  }

  void init_from_external() {
    if (options_.external_key) {
      external_key_ = options_.external_key.get(context_);
    } else {
      external_key_ = std::string{cookies_.get(options_.key)};
    }

    if (external_key_.empty()) {
      external_key_ = options_.genid();
    }

    const auto json = options_.store.get(external_key_);
    if (!json.empty() && parse(json)) {
      pre_json_ = to_string();
    }
  }

  void destroy() {
    if (options_.store) {
      options_.store.destroy(external_key_);
    }

    if (options_.external_key) {
      options_.external_key.destroy(context_, external_key_);
    } else {
      cookie::options options;
      options.expires = std::string{detail::g_cookie_expires_date};
      cookies_.set(options_.key, "", std::move(options));
    }
  }

  bool parse(std::string_view json) {
    if (json.empty()) {
      return false;
    }

    using namespace nlohmann;
    const auto root = json::parse(json);
    if (!root.empty() && root.count("_expire") == 1 && root["_expire"].get<std::int64_t>() < detail::utils::now()) {
      return false;
    }

    for (const auto& item : root.items()) {
      if (item.key()[0] == '_') {
        continue;
      }
      datas_.emplace(item.key(), item.value());
    }
    return true;
  }

  nlohmann::json to_json() {
    nlohmann::json root;
    for (const auto& item : datas_) {
      root[item.first] = item.second;
    }
    return root;
  }

  std::string to_string() { return to_json().dump(); }

  options options_;
  context& context_;
  cookies& cookies_;
  std::string external_key_;
  std::map<std::string, std::string> datas_;
  std::string pre_json_;
  bool remove_{false};
};

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_SESSION_HPP_

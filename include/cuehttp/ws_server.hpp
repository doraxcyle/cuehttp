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

#ifndef CUEHTTP_WS_SERVER_HPP_
#define CUEHTTP_WS_SERVER_HPP_

#include <mutex>
#include <set>

#include "cuehttp/detail/middlewares.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/sha1.hpp"

namespace cue {
namespace http {

static constexpr std::string_view ws_magic_key{"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"};

class ws_server final : safe_noncopyable {
 public:
  ws_server() noexcept { callback_ = middlewares_.callback(); }

  template <typename... _Args>
  ws_server& use(_Args&&... args) {
    middlewares_.use(std::forward<_Args>(args)...);
    return *this;
  }

  void broadcast(std::string_view msg, ws_send::options options = {}) {
    std::unique_lock<std::mutex> lock{clients_mutex_};
    for (const auto& client : clients_) {
      client->send(std::string{msg}, options);
    }
  }

  std::function<void(context&)> callback() noexcept {
    return [this](context& ctx) {
      ctx.websocket().on_open([this, &ctx]() {
        std::unique_lock<std::mutex> lock{clients_mutex_};
        clients_.emplace(ctx.websocket().shared());
      });

      ctx.websocket().on_close([this, &ctx]() {
        std::unique_lock<std::mutex> lock{clients_mutex_};
        clients_.erase(ctx.websocket().shared());
      });

      // call middlewares
      callback_(ctx);

      if (ctx.status() != 404) {
        return;
      }
      // websocket handshake response
      std::string key{ctx.get("Sec-WebSocket-Key")};
      key.append(ws_magic_key.data(), ws_magic_key.size());
      unsigned char hash[20]{0};
      detail::sha1::calc(key.data(), key.size(), hash);
      std::string accept_key = detail::utils::base64_encode({reinterpret_cast<char*>(hash), 20});
      ctx.set("Sec-WebSocket-Accept", std::move(accept_key));
      ctx.set("Connection", "Upgrade");
      ctx.set("Upgrade", "WebSocket");
      ctx.status(101);
    };
  }

 private:
  detail::middlewares middlewares_;
  std::function<void(context&)> callback_;
  std::set<std::shared_ptr<websocket>> clients_;
  std::mutex clients_mutex_;
};

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_WS_SERVER_HPP_

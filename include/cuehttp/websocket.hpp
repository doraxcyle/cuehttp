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

#ifndef CUEHTTP_WEBSOCKET_HPP_
#define CUEHTTP_WEBSOCKET_HPP_

#include <functional>
#include <memory>
#include <vector>

#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {

namespace detail {

enum class ws_event : std::uint8_t { open = 0, close = 1, msg = 2 };

}  // namespace detail

namespace ws_send {

struct options final {
  bool fin{true};
  bool mask{true};
  bool binary{false};
};

}  // namespace ws_send

class websocket final : public std::enable_shared_from_this<websocket>, safe_noncopyable {
 public:
  explicit websocket(detail::ws_send_handler handler) noexcept : send_handler_{std::move(handler)} {}

  auto shared() { return shared_from_this(); }

  template <typename _Func>
  void on_open(_Func&& func) {
    open_handlers_.emplace_back(std::forward<_Func>(func));
  }

  template <typename _Func>
  void on_close(_Func&& func) {
    close_handlers_.emplace_back(std::forward<_Func>(func));
  }

  template <typename _Func>
  void on_message(_Func&& func) {
    msg_handlers_.emplace_back(std::forward<_Func>(func));
  }

  template <typename _Msg>
  void send(_Msg&& msg, ws_send::options options = {}) {
    send_handler_({options.fin, options.binary ? detail::ws_opcode::binary : detail::ws_opcode::text, options.mask,
                   std::forward<_Msg>(msg)});
  }

  void close() {
    detail::ws_frame frame;
    frame.opcode = detail::ws_opcode::close;
    send_handler_(std::move(frame));
  }

  void emit(detail::ws_event event, std::string&& msg = "") {
    switch (event) {
      case detail::ws_event::open:
        for (const auto& handler : open_handlers_) {
          handler();
        }
        break;
      case detail::ws_event::close:
        for (const auto& handler : close_handlers_) {
          handler();
        }
        break;
      case detail::ws_event::msg:
        if (!msg_handlers_.empty()) {
          const auto last_index = msg_handlers_.size() - 1;
          for (std::size_t i{0}; i < last_index; ++i) {
            msg_handlers_[i](std::string{msg});
          }
          msg_handlers_[last_index](std::move(msg));
        }
        break;
      default:
        break;
    }
  }

 private:
  std::vector<std::function<void()>> open_handlers_;
  std::vector<std::function<void()>> close_handlers_;
  std::vector<std::function<void(std::string&&)>> msg_handlers_;
  detail::ws_send_handler send_handler_;
};

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_WEBSOCKET_HPP_

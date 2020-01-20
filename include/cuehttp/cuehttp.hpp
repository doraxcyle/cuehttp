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

#ifndef CUEHTTP_CUEHTTP_HPP_
#define CUEHTTP_CUEHTTP_HPP_

#include <memory>

#include "cuehttp/server.hpp"
#include "cuehttp/ws_server.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/engines.hpp"
#include "cuehttp/detail/middlewares.hpp"

namespace cue {
namespace http {

class cuehttp final : safe_noncopyable {
public:
    cuehttp() noexcept {
        http_handler_ = middlewares_.callback();
        ws_handler_ = [](context& ctx) { ctx.status(503); };
    }

    inline static void run() {
        detail::engines::default_engines().run();
    }

    inline static void stop() {
        detail::engines::default_engines().stop();
    }

    cuehttp& listen(unsigned port) {
        assert(port != 0);
        server_ = http::create_server(callback());
        server_.listen(port);
        return *this;
    }

    template <typename Host>
    cuehttp& listen(unsigned port, Host&& host) {
        assert(port != 0);
        server_ = http::create_server(callback());
        server_.listen(port, std::forward<Host>(host));
        return *this;
    }

    std::function<void(context&)> callback() const noexcept {
        return [this](context& ctx) {
            if (ctx.req().websocket()) {
                ws_handler_(ctx);
            } else {
                http_handler_(ctx);
            }
        };
    }

    template <typename... Args>
    cuehttp& use(Args&&... args) {
        middlewares_.use(std::forward<Args>(args)...);
        return *this;
    }

    ws_server& ws() {
        if (!ws_) {
            ws_ = std::make_unique<ws_server>();
            ws_handler_ = ws_->callback();
        }
        return *ws_;
    }

private:
    http_t server_;
    detail::middlewares middlewares_;
    std::function<void(context&)> http_handler_;
    // websocket
    std::unique_ptr<ws_server> ws_;
    std::function<void(context&)> ws_handler_;
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_CUEHTTP_HPP_

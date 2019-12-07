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

#ifndef CUEHTTP_SERVER_HPP_
#define CUEHTTP_SERVER_HPP_

#include <thread>
#include <functional>
#include <vector>
#include <type_traits>
#include <boost/asio.hpp>

#include "cuehttp/server_options.hpp"
#include "cuehttp/context.hpp"
#include "cuehttp/detail/connection.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/engines.hpp"

namespace cue {
namespace http {

class server final : safe_noncopyable {
public:
    server() noexcept : engines_{server_options::instance().pool_size} {
        compose();
    }

    ~server() noexcept = default;

    void listen(unsigned port) {
        assert(port != 0);
        listen_and_run(boost::asio::ip::tcp::resolver::query{std::to_string(port)});
    }

    template <typename T>
    void listen(unsigned port, T&& t) {
        assert(port != 0);
        listen_and_run(boost::asio::ip::tcp::resolver::query{std::forward<T>(t), std::to_string(port)});
    }

    void stop() {
        engines_.stop();
    }

    template <typename... Args>
    server& use(Args&&... args) {
        use_impl(std::forward<Args>(args)...);
        return *this;
    }

private:
    template <typename T, typename = std::enable_if_t<detail::is_middleware_list<T>::value>>
    void use_impl(T&& t) {
        use_append_list(std::forward<T>(t));
    }

    void use_append_list(const std::vector<std::function<void(context& ctx, std::function<void()> next)>>& handlers) {
        middlewares_.insert(middlewares_.end(), handlers.begin(), handlers.end());
    }

    void use_append_list(std::vector<std::function<void(context& ctx, std::function<void()> next)>>&& handlers) {
        middlewares_.insert(middlewares_.end(), std::make_move_iterator(handlers.begin()),
                            std::make_move_iterator(handlers.end()));
    }

    template <typename Func, typename = std::enable_if_t<!std::is_member_function_pointer<Func>::value>>
    std::enable_if_t<detail::is_middleware<Func>::value, std::true_type> use_impl(Func&& func) {
        use_with_next(std::forward<Func>(func));
        return std::true_type{};
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>>
    void use_impl(Func (T::*func)(context&, std::function<void()>), Self self) {
        use_with_next(func, self);
    }

    template <typename T, typename Func>
    void use_impl(Func (T::*func)(context&, std::function<void()>)) noexcept {
        use_with_next(func, (T*)nullptr);
    }

    template <typename Func, typename = std::enable_if_t<!std::is_member_function_pointer<Func>::value>>
    std::enable_if_t<detail::is_middleware_without_next<Func>::value, std::false_type> use_impl(Func&& func) {
        use_without_next(std::forward<Func>(func));
        return std::false_type{};
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>>
    void use_impl(Func (T::*func)(context&), Self self) {
        use_without_next(func, self);
    }

    template <typename T, typename Func>
    void use_impl(Func (T::*func)(context&)) {
        use_without_next(func, (T*)nullptr);
    }

    template <typename Func>
    void use_with_next(Func&& func) {
        middlewares_.emplace_back(std::forward<Func>(func));
    }

    template <typename T, typename Func, typename Self>
    void use_with_next(Func T::*func, Self self) {
        middlewares_.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (*self.*func)(ctx, std::move(next));
            } else {
                (T{}.*func)(ctx, std::move(next));
            }
        });
    }

    template <typename Func>
    void use_without_next(Func&& func) {
        middlewares_.emplace_back(
            std::bind(&server::call_with_next, this, func, std::placeholders::_1, std::placeholders::_2));
    }

    template <typename T, typename Func, typename Self>
    void use_without_next(Func T::*func, Self self) {
        middlewares_.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (*self.*func)(ctx);
            } else {
                (T{}.*func)(ctx);
            }
            next();
        });
    }

    void call_with_next(std::function<void(context&)> func, context& ctx, std::function<void()> next) {
        func(ctx);
        next();
    }

    void compose() noexcept {
        handler_ = [this](context& ctx) {
            if (middlewares_.empty()) {
                return;
            }

            size_t index{0};
            std::function<void()> next;
            next = [this, &next, &index, &ctx]() {
                if (++index == middlewares_.size()) {
                    return;
                }
                middlewares_.at(index)(ctx, next);
            };

            middlewares_.at(0)(ctx, next);
        };
    }

    void listen_and_run(boost::asio::ip::tcp::resolver::query&& query) {
        boost::asio::ip::tcp::endpoint endpoint{*boost::asio::ip::tcp::resolver{engines_.get()}.resolve(query)};
        acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(engines_.get());
        acceptor_->open(endpoint.protocol());
        acceptor_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_->bind(endpoint);
        acceptor_->listen();
        do_accept();

        engines_.run();
    }

    void do_accept() {
        auto connector = std::make_shared<detail::connection>(engines_.get(), handler_);
        acceptor_->async_accept(connector->socket(), [this, connector](boost::system::error_code code) {
            if (!acceptor_->is_open()) {
                return;
            }

            if (!code) {
                connector->start();
            }

            do_accept();
        });
    }

    detail::engines engines_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    std::function<void(context&)> handler_;
    std::vector<std::function<void(context&, std::function<void()>)>> middlewares_;
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_SERVER_HPP_

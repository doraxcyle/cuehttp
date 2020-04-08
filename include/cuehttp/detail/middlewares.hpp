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

#ifndef CUEHTTP_MIDDLEWARES_HPP_
#define CUEHTTP_MIDDLEWARES_HPP_

#include <functional>
#include <vector>

#include "cuehttp/context.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {
namespace detail {

class middlewares final : safe_noncopyable {
public:
    middlewares() noexcept {
        compose();
    }

    const std::function<void(context&)>& callback() const noexcept {
        return handler_;
    }

    template <typename... Args>
    void use(Args&&... args) {
        use_impl(std::forward<Args>(args)...);
    }

private:
    template <typename List, typename = std::enable_if_t<detail::is_middleware_list<List>::value>>
    void use_impl(List&& list) {
        use_append_list(std::forward<List>(list));
    }

    void use_append_list(const std::vector<std::function<void(context&, std::function<void()>)>>& handlers) {
        middlewares_.insert(middlewares_.end(), handlers.begin(), handlers.end());
    }

    void use_append_list(std::vector<std::function<void(context&, std::function<void()>)>>&& handlers) {
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
                (self->*func)(ctx, std::move(next));
            } else {
                (T{}.*func)(ctx, std::move(next));
            }
        });
    }

    template <typename Func>
    void use_without_next(Func&& func) {
        middlewares_.emplace_back(std::bind(&middlewares::call_with_next, this,
                                            // for operator std::function<void(context&)>
                                            static_cast<std::function<void(context&)>>(func), std::placeholders::_1,
                                            std::placeholders::_2));
    }

    template <typename T, typename Func, typename Self>
    void use_without_next(Func T::*func, Self self) {
        middlewares_.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (self->*func)(ctx);
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

            std::size_t index{0};
            std::function<void()> next;
            next = [this, &next, &index, &ctx]() {
                if (++index == middlewares_.size()) {
                    return;
                }
                middlewares_[index](ctx, next);
            };

            middlewares_[0](ctx, next);
        };
    }

    std::function<void(context&)> handler_;
    std::vector<std::function<void(context&, std::function<void()>)>> middlewares_;
};

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_MIDDLEWARES_HPP_

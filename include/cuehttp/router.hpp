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

#ifndef CUEHTTP_ROUTER_HPP_
#define CUEHTTP_ROUTER_HPP_

#include <functional>
#include <unordered_map>
#include <boost/noncopyable.hpp>

#include "cuehttp/context.hpp"
#include "cuehttp/detail/common.hpp"

namespace cue {
namespace http {

class router final : public boost::noncopyable {
public:
    router() noexcept = default;

    template <typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, router>::value>>
    explicit router(T&& t) noexcept : prefix_{std::forward<T>(t)} {
    }

    std::function<void(context&)> routes() const noexcept {
        return make_routes();
    }

    template <typename T>
    router& prefix(T&& t) {
        prefix_ = std::forward<T>(t);
        return *this;
    }

    template <typename... Args>
    router& del(const std::string& path, Args&&... args) {
        register_impl("del", path, std::forward<Args>(args)...);
        return *this;
    }

    template <typename... Args>
    router& get(const std::string& path, Args&&... args) {
        register_impl("get", path, std::forward<Args>(args)...);
        return *this;
    }

    template <typename... Args>
    router& head(const std::string& path, Args&&... args) {
        register_impl("head", path, std::forward<Args>(args)...);
        return *this;
    }

    template <typename... Args>
    router& post(const std::string& path, Args&&... args) {
        register_impl("post", path, std::forward<Args>(args)...);
        return *this;
    }

    template <typename... Args>
    router& put(const std::string& path, Args&&... args) {
        register_impl("put", path, std::forward<Args>(args)...);
        return *this;
    }

    template <typename... Args>
    router& all(const std::string& path, Args&&... args) {
        static const std::vector<std::string> methods{"del", "get", "head", "post", "put"};
        for (std::string method : methods) {
            register_impl(std::move(method), path, std::forward<Args>(args)...);
        }
        return *this;
    }

    template <typename... Args>
    router& redirect(Args&&... args) {
        redirect_impl(std::forward<Args>(args)...);
        return *this;
    }

private:
    template <typename Func, typename = std::enable_if_t<!std::is_member_function_pointer<Func>::value>,
              typename... Args>
    void register_impl(std::string&& method, const std::string& path, Func&& func, Args&&... args) {
        std::vector<std::function<void(context&, std::function<void()>)>> handlers;
        register_multiple(handlers, std::forward<Func>(func), std::forward<Args>(args)...);
        compose(std::move(method), path, std::move(handlers));
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>,
              typename... Args>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&, std::function<void()>),
                       Self self, Args&&... args) {
        std::vector<std::function<void(context&, std::function<void()>)>> handlers;
        register_multiple(handlers, func, self, std::forward<Args>(args)...);
        compose(std::move(method), path, std::move(handlers));
    }

    template <typename T, typename Func, typename... Args>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&, std::function<void()>),
                       Args&&... args) {
        std::vector<std::function<void(context&, std::function<void()>)>> handlers;
        register_multiple(handlers, func, std::forward<Args>(args)...);
        compose(std::move(method), path, std::move(handlers));
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>,
              typename... Args>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&), Self self,
                       Args&&... args) {
        std::vector<std::function<void(context&, std::function<void()>)>> handlers;
        register_multiple(handlers, func, self, std::forward<Args>(args)...);
        compose(std::move(method), path, std::move(handlers));
    }

    template <typename T, typename Func, typename... Args>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&), Args&&... args) {
        std::vector<std::function<void(context&, std::function<void()>)>> handlers;
        register_multiple(handlers, func, std::forward<Args>(args)...);
        compose(std::move(method), path, std::move(handlers));
    }

    template <typename Func, typename... Args>
    std::enable_if_t<detail::is_middleware<Func>::value, std::true_type> register_multiple(
        std::vector<std::function<void(context&, std::function<void()>)>>& handlers, Func&& func, Args&&... args) {
        handlers.emplace_back(std::forward<Func>(func));
        register_multiple(handlers, std::forward<Args>(args)...);
        return std::true_type{};
    }

    template <typename Func, typename... Args>
    std::enable_if_t<detail::is_middleware<Func>::value, std::true_type> register_multiple(
        std::vector<std::function<void(context&, std::function<void()>)>>& handlers, Func&& func) {
        handlers.emplace_back(std::forward<Func>(func));
        return std::true_type{};
    }

    template <typename Func, typename... Args>
    std::enable_if_t<detail::is_middleware_without_next<Func>::value, std::false_type> register_multiple(
        std::vector<std::function<void(context&, std::function<void()>)>>& handlers, Func&& func, Args&&... args) {
        handlers.emplace_back([func = std::forward<Func>(func)](context& ctx, std::function<void()> next) {
            func(ctx);
            next();
        });
        register_multiple(handlers, std::forward<Args>(args)...);
        return std::false_type{};
    }

    template <typename Func, typename... Args>
    std::enable_if_t<detail::is_middleware_without_next<Func>::value, std::false_type> register_multiple(
        std::vector<std::function<void(context&, std::function<void()>)>>& handlers, Func&& func) {
        handlers.emplace_back([func = std::forward<Func>(func)](context& ctx, std::function<void()> next) {
            func(ctx);
            next();
        });
        return std::false_type{};
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>,
              typename... Args>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&, std::function<void()>), Self self, Args&&... args) {
        handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (*self.*func)(ctx, std::move(next));
            }
        });
        register_multiple(handlers, std::forward<Args>(args)...);
    }

    template <typename T, typename Func, typename... Args>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&, std::function<void()>), Args&&... args) {
        handlers.emplace_back([func](context& ctx, std::function<void()> next) { (T{}.*func)(ctx, std::move(next)); });
        register_multiple(handlers, std::forward<Args>(args)...);
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>,
              typename... Args>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&), Self self, Args&&... args) {
        handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (*self.*func)(ctx);
            }
            next();
        });
        register_multiple(handlers, std::forward<Args>(args)...);
    }

    template <typename T, typename Func, typename... Args>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&), Args&&... args) {
        handlers.emplace_back([func](context& ctx, std::function<void()> next) {
            (T{}.*func)(ctx);
            next();
        });
        register_multiple(handlers, std::forward<Args>(args)...);
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&, std::function<void()>), Self self) {
        handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (*self.*func)(ctx, std::move(next));
            }
        });
    }

    template <typename T, typename Func>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&, std::function<void()>)) {
        handlers.emplace_back([func](context& ctx, std::function<void()> next) { (T{}.*func)(ctx, std::move(next)); });
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&), Self self) {
        handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
            if (self) {
                (*self.*func)(ctx);
            }
        });
    }

    template <typename T, typename Func>
    void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                           Func (T::*func)(context&)) {
        handlers.emplace_back([func](context& ctx, std::function<void()> next) { (T{}.*func)(ctx); });
    }

    template <typename Func, typename = std::enable_if_t<!std::is_member_function_pointer<Func>::value>>
    std::enable_if_t<detail::is_middleware<Func>::value, std::true_type> register_impl(std::string&& method,
                                                                                       const std::string& path,
                                                                                       Func&& func) {
        register_with_next(std::move(method), path, std::forward<Func>(func));
        return std::true_type{};
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&, std::function<void()>),
                       Self self) {
        register_with_next(std::move(method), path, func, self);
    }

    template <typename T, typename Func>
    void register_impl(std::string&& method, const std::string& path,
                       Func (T::*func)(context&, std::function<void()>)) {
        register_with_next(std::move(method), path, func, (T*)nullptr);
    }

    template <typename Func, typename = std::enable_if_t<!std::is_member_function_pointer<Func>::value>>
    std::enable_if_t<detail::is_middleware_without_next<Func>::value, std::false_type> register_impl(
        std::string&& method, const std::string& path, Func&& func) {
        register_without_next(std::move(method), path, std::forward<Func>(func));
        return std::false_type{};
    }

    template <typename T, typename Func, typename Self, typename = std::enable_if_t<std::is_same<T*, Self>::value>>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&), Self self) {
        register_without_next(std::move(method), path, func, self);
    }

    template <typename T, typename Func>
    void register_impl(std::string&& method, const std::string& path, Func (T::*func)(context&)) {
        register_without_next(std::move(method), path, func, (T*)nullptr);
    }

    template <typename Func>
    void register_with_next(std::string&& method, const std::string& path, Func&& func) {
        handlers_.emplace(std::move(method + "+" + prefix_ + detail::utils::to_lower(path)),
                          [func = std::forward<Func>(func)](context& ctx) {
                              const auto next = []() {};
                              func(ctx, std::move(next));
                          });
    }

    template <typename T, typename Func, typename Self>
    void register_with_next(std::string&& method, const std::string& path, Func T::*func, Self self) {
        handlers_.emplace(std::move(method + "+" + prefix_ + detail::utils::to_lower(path)),
                          [func, self](context& ctx) {
                              const auto next = []() {};
                              if (self) {
                                  (*self.*func)(ctx, std::move(next));
                              } else {
                                  (T{}.*func)(ctx, std::move(next));
                              }
                          });
    }

    template <typename Func>
    void register_without_next(std::string&& method, const std::string& path, Func&& func) {
        handlers_.emplace(std::move(method + "+" + prefix_ + detail::utils::to_lower(path)), std::forward<Func>(func));
    }

    template <typename T, typename Func, typename Self>
    void register_without_next(std::string&& method, const std::string& path, Func T::*func, Self self) {
        handlers_.emplace(std::move(method + "+" + prefix_ + detail::utils::to_lower(path)),
                          [func, self](context& ctx) {
                              if (self) {
                                  (*self.*func)(ctx);
                              } else {
                                  (T{}.*func)(ctx);
                              }
                          });
    }

    template <typename T>
    void redirect_impl(const std::string& path, T&& t) {
        redirect_impl(path, std::forward<T>(t), 301);
    }

    template <typename T>
    void redirect_impl(const std::string& path, T&& t, unsigned status) {
        all(path, [destination = std::forward<T>(t), status](context& ctx) {
            ctx.redirect(std::move(destination));
            ctx.status(status);
        });
    }

    void compose(std::string&& method, const std::string& path,
                 std::vector<std::function<void(context&, std::function<void()>)>>&& handlers) {
        const auto handler = [this, handlers = std::move(handlers)](context& ctx) {
            if (handlers.empty()) {
                return;
            }

            size_t index{0};
            std::function<void()> next;
            next = [&handlers, &next, &index, &ctx]() {
                if (++index == handlers.size()) {
                    return;
                }
                handlers.at(index)(ctx, next);
            };

            handlers.at(0)(ctx, next);
        };
        handlers_.emplace(std::move(method + "+" + prefix_ + detail::utils::to_lower(path)), std::move(handler));
    }

    std::function<void(context&)> make_routes() const noexcept {
        return [this](context& ctx) {
            if (ctx.status() != 404) {
                return;
            }
            const std::string key{detail::utils::to_lower(ctx.method()) + "+" + prefix_ +
                                  detail::utils::to_lower(ctx.path())};
            const auto it = handlers_.find(key);
            if (it != handlers_.end()) {
                it->second(ctx);
                return;
            }
            ctx.status(404);
        };
    }

    std::string prefix_;
    std::unordered_map<std::string, std::function<void(context&)>> handlers_;
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_ROUTER_HPP_

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

#include "cuehttp/context.hpp"
#include "cuehttp/deps/fmt/fmt.h"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {

using namespace std::string_view_literals;

class router final : safe_noncopyable {
 public:
  router() noexcept = default;

  template <typename _Prefix, typename = std::enable_if_t<!std::is_same_v<std::decay_t<_Prefix>, router>>>
  explicit router(_Prefix&& prefix) noexcept : prefix_{std::forward<_Prefix>(prefix)} {}

  std::function<void(context&)> routes() const noexcept { return make_routes(); }

  template <typename _Prefix>
  router& prefix(_Prefix&& prefix) {
    prefix_ = std::forward<_Prefix>(prefix);
    return *this;
  }

  template <typename... _Args>
  router& del(std::string_view path, _Args&&... args) {
    register_impl("DEL"sv, path, std::forward<_Args>(args)...);
    return *this;
  }

  template <typename... _Args>
  router& get(std::string_view path, _Args&&... args) {
    register_impl("GET"sv, path, std::forward<_Args>(args)...);
    return *this;
  }

  template <typename... _Args>
  router& head(std::string_view path, _Args&&... args) {
    register_impl("HEAD"sv, path, std::forward<_Args>(args)...);
    return *this;
  }

  template <typename... _Args>
  router& post(std::string_view path, _Args&&... args) {
    register_impl("POST"sv, path, std::forward<_Args>(args)...);
    return *this;
  }

  template <typename... _Args>
  router& put(std::string_view path, _Args&&... args) {
    register_impl("PUT"sv, path, std::forward<_Args>(args)...);
    return *this;
  }

  template <typename... _Args>
  router& all(std::string_view path, _Args&&... args) {
    static constexpr std::array<std::string_view, 5> methods{"DEL"sv, "GET"sv, "HEAD"sv, "POST"sv, "PUT"sv};
    for (const auto& method : methods) {
      register_impl(method, path, std::forward<_Args>(args)...);
    }
    return *this;
  }

  template <typename... _Args>
  router& redirect(_Args&&... args) {
    redirect_impl(std::forward<_Args>(args)...);
    return *this;
  }

  operator auto() const noexcept { return make_routes(); }

 private:
  template <typename _Func, typename = std::enable_if_t<!std::is_member_function_pointer_v<_Func>>, typename... _Args>
  void register_impl(std::string_view method, std::string_view path, _Func&& func, _Args&&... args) {
    std::vector<std::function<void(context&, std::function<void()>)>> handlers;
    register_multiple(handlers, std::forward<_Func>(func), std::forward<_Args>(args)...);
    compose(method, path, std::move(handlers));
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>,
            typename... _Args>
  void register_impl(std::string_view method, std::string_view path,
                     _Func (_Ty::*func)(context&, std::function<void()>), _Self self, _Args&&... args) {
    std::vector<std::function<void(context&, std::function<void()>)>> handlers;
    register_multiple(handlers, func, self, std::forward<_Args>(args)...);
    compose(method, path, std::move(handlers));
  }

  template <typename _Ty, typename _Func, typename... _Args>
  void register_impl(std::string_view method, std::string_view path,
                     _Func (_Ty::*func)(context&, std::function<void()>), _Args&&... args) {
    std::vector<std::function<void(context&, std::function<void()>)>> handlers;
    register_multiple(handlers, func, std::forward<_Args>(args)...);
    compose(method, path, std::move(handlers));
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>,
            typename... _Args>
  void register_impl(std::string_view method, std::string_view path, _Func (_Ty::*func)(context&), _Self self,
                     _Args&&... args) {
    std::vector<std::function<void(context&, std::function<void()>)>> handlers;
    register_multiple(handlers, func, self, std::forward<_Args>(args)...);
    compose(method, path, std::move(handlers));
  }

  template <typename _Ty, typename _Func, typename... _Args>
  void register_impl(std::string_view method, std::string_view path, _Func (_Ty::*func)(context&), _Args&&... args) {
    std::vector<std::function<void(context&, std::function<void()>)>> handlers;
    register_multiple(handlers, func, std::forward<_Args>(args)...);
    compose(method, path, std::move(handlers));
  }

  template <typename _Func, typename... _Args>
  std::enable_if_t<detail::is_middleware_v<_Func>, std::true_type> register_multiple(
      std::vector<std::function<void(context&, std::function<void()>)>>& handlers, _Func&& func, _Args&&... args) {
    handlers.emplace_back(std::forward<_Func>(func));
    register_multiple(handlers, std::forward<_Args>(args)...);
    return std::true_type{};
  }

  template <typename _Func, typename... _Args>
  std::enable_if_t<detail::is_middleware_v<_Func>, std::true_type> register_multiple(
      std::vector<std::function<void(context&, std::function<void()>)>>& handlers, _Func&& func) {
    handlers.emplace_back(std::forward<_Func>(func));
    return std::true_type{};
  }

  template <typename _Func, typename... _Args>
  std::enable_if_t<detail::is_middleware_without_next_v<_Func>, std::false_type> register_multiple(
      std::vector<std::function<void(context&, std::function<void()>)>>& handlers, _Func&& func, _Args&&... args) {
    handlers.emplace_back([func = std::forward<_Func>(func)](context& ctx, std::function<void()> next) {
      func(ctx);
      next();
    });
    register_multiple(handlers, std::forward<_Args>(args)...);
    return std::false_type{};
  }

  template <typename _Func, typename... _Args>
  std::enable_if_t<detail::is_middleware_without_next_v<_Func>, std::false_type> register_multiple(
      std::vector<std::function<void(context&, std::function<void()>)>>& handlers, _Func&& func) {
    handlers.emplace_back([func = std::forward<_Func>(func)](context& ctx, std::function<void()> next) {
      func(ctx);
      next();
    });
    return std::false_type{};
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>,
            typename... _Args>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&, std::function<void()>), _Self self, _Args&&... args) {
    handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
      if (self) {
        (self->*func)(ctx, std::move(next));
      }
    });
    register_multiple(handlers, std::forward<_Args>(args)...);
  }

  template <typename _Ty, typename _Func, typename... _Args>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&, std::function<void()>), _Args&&... args) {
    handlers.emplace_back([func](context& ctx, std::function<void()> next) { (_Ty{}.*func)(ctx, std::move(next)); });
    register_multiple(handlers, std::forward<_Args>(args)...);
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>,
            typename... _Args>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&), _Self self, _Args&&... args) {
    handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
      if (self) {
        (self->*func)(ctx);
      }
      next();
    });
    register_multiple(handlers, std::forward<_Args>(args)...);
  }

  template <typename _Ty, typename _Func, typename... _Args>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&), _Args&&... args) {
    handlers.emplace_back([func](context& ctx, std::function<void()> next) {
      (_Ty{}.*func)(ctx);
      next();
    });
    register_multiple(handlers, std::forward<_Args>(args)...);
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&, std::function<void()>), _Self self) {
    handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
      if (self) {
        (self->*func)(ctx, std::move(next));
      }
    });
  }

  template <typename _Ty, typename _Func>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&, std::function<void()>)) {
    handlers.emplace_back([func](context& ctx, std::function<void()> next) { (_Ty{}.*func)(ctx, std::move(next)); });
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&), _Self self) {
    handlers.emplace_back([func, self](context& ctx, std::function<void()> next) {
      if (self) {
        (self->*func)(ctx);
      }
    });
  }

  template <typename _Ty, typename _Func>
  void register_multiple(std::vector<std::function<void(context&, std::function<void()>)>>& handlers,
                         _Func (_Ty::*func)(context&)) {
    handlers.emplace_back([func](context& ctx, std::function<void()> next) { (_Ty{}.*func)(ctx); });
  }

  template <typename _Func, typename = std::enable_if_t<!std::is_member_function_pointer_v<_Func>>>
  std::enable_if_t<detail::is_middleware_v<_Func>, std::true_type> register_impl(std::string_view method,
                                                                                 std::string_view path, _Func&& func) {
    register_with_next(method, path, std::forward<_Func>(func));
    return std::true_type{};
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>>
  void register_impl(std::string_view method, std::string_view path,
                     _Func (_Ty::*func)(context&, std::function<void()>), _Self self) {
    register_with_next(method, path, func, self);
  }

  template <typename _Ty, typename _Func>
  void register_impl(std::string_view method, std::string_view path,
                     _Func (_Ty::*func)(context&, std::function<void()>)) {
    register_with_next(method, path, func, (_Ty*)nullptr);
  }

  template <typename _Func, typename = std::enable_if_t<!std::is_member_function_pointer_v<_Func>>>
  std::enable_if_t<detail::is_middleware_without_next_v<_Func>, std::false_type> register_impl(std::string_view method,
                                                                                               std::string_view path,
                                                                                               _Func&& func) {
    register_without_next(method, path, std::forward<_Func>(func));
    return std::false_type{};
  }

  template <typename _Ty, typename _Func, typename _Self, typename = std::enable_if_t<std::is_same_v<_Ty*, _Self>>>
  void register_impl(std::string_view method, std::string_view path, _Func (_Ty::*func)(context&), _Self self) {
    register_without_next(method, path, func, self);
  }

  template <typename _Ty, typename _Func>
  void register_impl(std::string_view method, std::string_view path, _Func (_Ty::*func)(context&)) {
    register_without_next(method, path, func, (_Ty*)nullptr);
  }

  template <typename _Func>
  void register_with_next(std::string_view method, std::string_view path, _Func&& func) {
    handlers_.emplace(fmt::format("{}+{}{}", method, prefix_, path), [func = std::forward<_Func>(func)](context& ctx) {
      const auto next = []() {};
      func(ctx, std::move(next));
    });
  }

  template <typename _Ty, typename _Func, typename _Self>
  void register_with_next(std::string_view method, std::string_view path, _Func _Ty::*func, _Self self) {
    handlers_.emplace(fmt::format("{}+{}{}", method, prefix_, path), [func, self](context& ctx) {
      const auto next = []() {};
      if (self) {
        (self->*func)(ctx, std::move(next));
      } else {
        (_Ty{}.*func)(ctx, std::move(next));
      }
    });
  }

  template <typename _Func>
  void register_without_next(std::string_view method, std::string_view path, _Func&& func) {
    handlers_.emplace(fmt::format("{}+{}{}", method, prefix_, path), std::forward<_Func>(func));
  }

  template <typename _Ty, typename _Func, typename _Self>
  void register_without_next(std::string_view method, std::string_view path, _Func _Ty::*func, _Self self) {
    handlers_.emplace(fmt::format("{}+{}{}", method, prefix_, path), [func, self](context& ctx) {
      if (self) {
        (self->*func)(ctx);
      } else {
        (_Ty{}.*func)(ctx);
      }
    });
  }

  template <typename _Dest>
  void redirect_impl(std::string_view path, _Dest&& destination) {
    redirect_impl(path, std::forward<_Dest>(destination), 301);
  }

  template <typename _Dest>
  void redirect_impl(std::string_view path, _Dest&& destination, unsigned status) {
    all(path, [destination = std::forward<_Dest>(destination), status](context& ctx) {
      ctx.redirect(std::move(destination));
      ctx.status(status);
    });
  }

  void compose(std::string_view method, std::string_view path,
               std::vector<std::function<void(context&, std::function<void()>)>>&& handlers) {
    const auto handler = [handlers = std::move(handlers)](context& ctx) {
      if (handlers.empty()) {
        return;
      }

      if (handlers.size() == 1) {
        handlers[0](ctx, []() {});
      } else {
        std::size_t index{0};
        std::function<void()> next;
        next = [&handlers, &next, &index, &ctx]() {
          if (++index == handlers.size()) {
            return;
          }
          handlers[index](ctx, next);
        };

        handlers[0](ctx, next);
      }
    };
    handlers_.emplace(fmt::format("{}+{}{}", method, prefix_, path), std::move(handler));
  }

  std::function<void(context&)> make_routes() const noexcept {
    return [this](context& ctx) {
      if (ctx.status() != 404) {
        return;
      }

      const auto it = handlers_.find(fmt::format("{}+{}{}", ctx.method(), prefix_, ctx.path()));
      if (it != handlers_.end()) {
        it->second(ctx);
      }
    };
  }

  std::string prefix_;
  std::unordered_map<std::string, std::function<void(context&)>> handlers_;
};

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_ROUTER_HPP_

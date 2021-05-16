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

#ifndef CUEHTTP_USE_SESSION_HPP_
#define CUEHTTP_USE_SESSION_HPP_

#include <functional>

#include "cuehttp/context.hpp"
#include "cuehttp/session.hpp"

namespace cue {
namespace http {

template <typename _Options>
inline auto use_session(_Options&& options) noexcept {
  return [options = std::forward<_Options>(options)](context& ctx, std::function<void()> next) {
    const bool auto_commit{options.auto_commit};
    if (!ctx.session_enabled()) {
      ctx.session(std::move(options));
    }
    // call next
    next();
    // auto commit session
    if (auto_commit) {
      ctx.session().commit();
    }
  };
}

}  // namespace http
}  // namespace cue

#endif  // CUE_HTTP_USE_SESSION_HPP_

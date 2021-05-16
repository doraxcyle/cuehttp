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

#ifndef CUEHTTP_STATIC_HPP_
#define CUEHTTP_STATIC_HPP_

#include "cuehttp/detail/static.hpp"

namespace cue {
namespace http {

template <typename... _Args>
inline auto use_static(_Args&&... args) noexcept {
  return detail::use_static(std::forward<_Args>(args)...);
}

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_STATIC_HPP_

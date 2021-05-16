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

#ifndef CUEHTTP_NONCOPYABLE_HPP_
#define CUEHTTP_NONCOPYABLE_HPP_

namespace cue {
namespace http {
namespace detail {

template <unsigned _Count>
class noncopyable_ {
 protected:
  noncopyable_() noexcept = default;
  ~noncopyable_() = default;

  noncopyable_(const noncopyable_&) = delete;
  noncopyable_& operator=(const noncopyable_&) = delete;
};

}  // namespace detail

#define safe_noncopyable \
 private                 \
  detail::noncopyable_<__COUNTER__>

}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_NONCOPYABLE_HPP_

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

#ifndef CUEHTTP_DETAIL_STATIC_HPP_
#define CUEHTTP_DETAIL_STATIC_HPP_

#include "cuehttp/context.hpp"
#include "cuehttp/send.hpp"

namespace cue {
namespace http {

namespace static_file {

struct options final {
    bool hidden{false};
    bool delay{false};
    std::string index;
    std::vector<std::string> extensions;
    bool cross_domain{false};
};

} // namespace static_file

namespace detail {

template <typename T, typename O>
static auto use_static(T&& t, O&& o) {
    return [root = std::forward<T>(t), options = std::forward<O>(o)](context& ctx, std::function<void()> next) {
        static const auto handler = [](context& ctx, std::string&& root, const static_file::options& static_options) {
            if (ctx.method() != "GET" && ctx.method() != "HEAD") {
                return;
            }
            send::options send_options;
            send_options.root = std::move(root);
            send_options.index = static_options.index.empty() ? "index.html" : std::move(static_options.index);
            send_options.extensions = std::move(static_options.extensions);
            send_options.hidden = static_options.hidden;
            send_options.cross_domain = static_options.cross_domain;
            send_file(ctx, ctx.path(), std::move(send_options));
        };

        if (options.delay) {
            next();
            if (ctx.status() != 404 || ctx.has_body()) {
                return;
            }
            handler(ctx, std::move(root), options);
        } else {
            handler(ctx, std::move(root), options);
            next();
        }
    };
}

template <typename T>
inline static auto use_static(T&& root) {
    return use_static(std::forward<T>(root), static_file::options{});
}

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_DETAIL_STATIC_HPP_

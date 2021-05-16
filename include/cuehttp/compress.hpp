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

#ifndef CUEHTTP_COMPRESS_HPP_
#define CUEHTTP_COMPRESS_HPP_

#ifdef ENABLE_GZIP

#include "cuehttp/context.hpp"
#include "cuehttp/detail/gzip.hpp"

namespace cue {
namespace http {

struct compress final {
  struct options final {
    std::uint64_t threshold{2048};
    int level{8};
  };

  static bool deflate(std::string_view src, std::string& dst, int level = 8) {
    return detail::gzip::compress(src, dst, level);
  }
};

template <typename _Options>
inline auto use_compress(_Options&& options) noexcept {
  return [options = std::forward<_Options>(options)](context& ctx, std::function<void()> next) {
    // call next
    next();

    if (ctx.req().method() == "HEAD") {
      return;
    }

    if (ctx.res().length() < options.threshold) {
      return;
    }

    const auto body = ctx.res().dump_body();
    std::string dst_body;
    if (!detail::gzip::compress(body, dst_body, options.level)) {
      ctx.status(500);
      return;
    }

    ctx.set("Content-Encoding", "gzip");
    ctx.body(std::move(dst_body));
  };
}

inline auto use_compress() noexcept { return use_compress(compress::options{}); }

}  // namespace http
}  // namespace cue

#endif  // ENABLE_GZIP

#endif  // CUEHTTP_COMPRESS_HPP_

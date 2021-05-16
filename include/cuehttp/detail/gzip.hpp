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

#ifndef CUEHTTP_GZIP_HPP_
#define CUEHTTP_GZIP_HPP_

#ifdef ENABLE_GZIP

#include <zlib.h>

#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {
namespace detail {

struct gzip final : safe_noncopyable {
  static bool compress(std::string_view src, std::string& dst, int level = 8) {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = Z_NULL;
    stream.next_in = Z_NULL;
    constexpr int flag{15 + 16};
    if (deflateInit2(&stream, level, Z_DEFLATED, flag, 9, Z_DEFAULT_STRATEGY) != Z_OK) {
      return false;
    }

    stream.next_in = (unsigned char*)src.data();
    stream.avail_in = static_cast<unsigned int>(src.length());

    do {
      unsigned char temp[4096];
      stream.next_out = temp;
      stream.avail_out = sizeof(temp);
      const auto code = deflate(&stream, Z_FINISH);
      dst.append((char*)temp, static_cast<unsigned>(stream.next_out - temp));
      if (code == Z_STREAM_END) {
        break;
      }
    } while (stream.avail_out == 0);

    return deflateEnd(&stream) == Z_OK;
  }
};

}  // namespace detail
}  // namespace http
}  // namespace cue

#endif  // ENABLE_GZIP

#endif  // CUEHTTP_GZIP_HPP_

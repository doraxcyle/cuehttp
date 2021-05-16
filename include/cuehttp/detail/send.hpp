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

#ifndef CUEHTTP_DETAIL_SEND_HPP_
#define CUEHTTP_DETAIL_SEND_HPP_

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "cuehttp/compress.hpp"
#include "cuehttp/context.hpp"
#include "cuehttp/detail/mime.hpp"

namespace cue {
namespace http {

namespace send {

struct options final {
  std::string root;
  bool hidden{false};
  std::string index;
  std::vector<std::string> extensions;
  std::uint64_t chunked_threshold{5 * 1024 * 1024};
  bool cross_domain{false};
  std::map<std::string, std::string> mime_types;
#ifdef ENABLE_GZIP
  bool gzip{true};
  std::uint64_t gzip_threshold{2048};
#endif  // ENABLE_GZIP
};

}  // namespace send

namespace detail {

template <typename _Path, typename _Options>
inline void send_file(context& ctx, _Path&& path, _Options&& options) {
  std::string temp_path = std::forward<_Path>(path);
  assert(!temp_path.empty());

  if (temp_path.back() == '/' && !options.index.empty()) {
    temp_path += options.index;
  }

  try {
    namespace fs = std::filesystem;
    fs::path real_path{options.root};
    real_path += temp_path;
    if (!options.hidden && real_path.filename().string()[0] == '.') {
      return;
    }

    if (!real_path.has_extension()) {
      for (const auto& item : options.extensions) {
        fs::path temp{real_path};
        temp.replace_extension(item);
        if (fs::exists(temp)) {
          real_path = temp;
          break;
        }
      }
    }

    if (!fs::exists(real_path) || !fs::is_regular_file(real_path)) {
      return;
    }

    std::ifstream file{real_path.string(), std::ios_base::binary};
    if (!file.is_open()) {
      return;
    }

    file.seekg(0, std::ios_base::end);
    auto file_size = file.tellg();
    if (file_size == -1) {
      return;
    }
    file.seekg(std::ios_base::beg);
#ifdef ENABLE_GZIP
    std::string data;
    if (static_cast<std::uint64_t>(file_size) >= options.gzip_threshold &&
        options.gzip) {
      std::ostringstream tmp;
      tmp << file.rdbuf();
      std::string dst;
      if (compress::deflate(tmp.str(), dst)) {
        data = std::move(dst);
        ctx.set("Content-Encoding", "gzip");
      }
      file_size = data.size();
    }
#endif  // ENABLE_GZIP
    if (options.cross_domain) {
      ctx.set("Access-Control-Allow-Origin", "*");
      ctx.set("Access-Control-Allow-Headers", "X-Requested-With");
      ctx.set("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
    }
    if (!ctx.res().has("Content-Type")) {
      const auto ext_str = real_path.extension().string();
      const auto it = options.mime_types.find(utils::to_lower(ext_str));
      if (it != options.mime_types.end()) {
        ctx.type(it->second);
      } else {
        ctx.type(get_mime(ext_str));
      }
    }
    ctx.status(200);
    if (static_cast<std::uint64_t>(file_size) >= options.chunked_threshold) {
      ctx.chunked();
    } else {
      ctx.length(file_size);
    }
#ifdef ENABLE_GZIP
    if (data.empty()) {
      ctx.body() << file.rdbuf();
    } else {
      ctx.body() << data;
    }
#else
    ctx.body() << file.rdbuf();
#endif  // ENABLE_GZIP
  } catch (...) {
    return;
  }
}

template <typename _Path>
inline void send_file(context& ctx, _Path&& path) {
  send_file(ctx, std::forward<_Path>(path), send::options{});
}

}  // namespace detail
}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_DETAIL_SEND_HPP_
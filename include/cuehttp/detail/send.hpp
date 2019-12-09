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

#include <vector>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

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
    std::size_t chunked_threshold{5 * 1024 * 1024};
};

} // namespace send

namespace detail {

template <typename T, typename O>
static void send_file(context& ctx, T&& t, O&& options) {
    std::string path = std::forward<T>(t);
    assert(!path.empty());

    if (path.back() == '/' && !options.index.empty()) {
        path += options.index;
    }

    try {
        namespace fs = boost::filesystem;
        fs::path real_path{options.root};
        real_path += path;
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

        const auto file_begin = file.tellg();
        file.seekg(0, std::ios_base::end);
        const auto file_size = file.tellg();
        file.seekg(file_begin);

        ctx.set("Access-Control-Allow-Origin", "*");
        ctx.set("Access-Control-Allow-Headers", "X-Requested-With");
        ctx.set("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
        if (!ctx.res().has("Content-Type")) {
            ctx.type(get_mime(real_path.extension().string()));
        }
        ctx.status(200);
        if (file_size > options.chunked_threshold) {
            ctx.set("Transfer-Encoding", "chunked");
        } else {
            ctx.length(file_size);
        }
        ctx.body() << file.rdbuf();
    } catch (...) {
        return;
    }
}

template <typename T>
inline static void send_file(context& ctx, T&& t) {
    send_file(ctx, std::forward<T>(t), send::options{});
}

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_DETAIL_SEND_HPP_
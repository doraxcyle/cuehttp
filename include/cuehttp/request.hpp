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

#ifndef CUEHTTP_REQUEST_HPP_
#define CUEHTTP_REQUEST_HPP_

#include <memory>
#include <vector>

#include "cuehttp/response.hpp"
#include "cuehttp/cookies.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/common.hpp"

namespace cue {
namespace http {

class request final : safe_noncopyable {
public:
    request(bool https, cookies& cookies) noexcept : https_{https}, cookies_{cookies} {
    }

    unsigned minor_version() const noexcept {
        return minor_version_;
    }

    std::string_view get(std::string_view field) const noexcept {
        for (const auto& header : headers_) {
            if (detail::utils::iequals(header.first, field)) {
                return header.second;
            }
        }
        using namespace std::literals;
        return ""sv;
    }

    const std::vector<std::pair<std::string_view, std::string_view>>& headers() const noexcept {
        return headers_;
    }

    std::string_view method() const noexcept {
        return method_;
    }

    std::string_view host() const noexcept {
        return get("Host");
    }

    std::string_view hostname() const noexcept {
        const auto host_str = host();
        return host_str.substr(0, host_str.rfind(":"));
    }

    std::string_view url() const noexcept {
        return url_;
    }

    std::string_view origin() const noexcept {
        if (origin_.empty()) {
            origin_ = https_ ? "https://" : "http://" + std::string{host()};
        }
        return origin_;
    }

    std::string_view href() const noexcept {
        if (href_.empty()) {
            href_ = std::string{origin()} + std::string{url_};
        }
        return href_;
    }

    std::string_view path() const noexcept {
        return path_;
    }

    std::string_view querystring() const noexcept {
        return querystring_;
    }

    const std::multimap<std::string, std::string>& query() const noexcept {
        if (!querystring_.empty() && query_.empty()) {
            query_ = detail::utils::parse_query(querystring_);
        }
        return query_;
    }

    std::string_view search() const noexcept {
        return search_;
    }

    std::string_view type() const noexcept {
        std::string_view content_type{get("Content-Type")};
        const auto pos = content_type.find("charset");
        if (pos != std::string_view::npos) {
            return content_type.substr(0, content_type.find(";"));
        } else {
            return content_type;
        }
    }

    std::string_view charset() const noexcept {
        std::string_view content_type{get("Content-Type")};
        const auto pos = content_type.find("charset");
        if (pos != std::string_view::npos) {
            return content_type.substr(pos + 8);
        } else {
            return content_type;
        }
    }

    std::uint64_t length() const noexcept {
        return content_length_;
    }

    bool websocket() const noexcept {
        return websocket_;
    }

    std::string_view body() const noexcept {
        return body_;
    }

    void reset() noexcept {
        origin_.clear();
        href_.clear();
        query_.clear();
        content_length_ = 0;
        websocket_ = false;
        headers_.clear();
    }

private:
    void parse_url() {
        const auto pos = url_.find('?');
        if (pos != std::string_view::npos) {
            path_ = url_.substr(0, pos);
            querystring_ = url_.substr(pos + 1, url_.length() - pos - 1);
            search_ = url_.substr(pos, url_.length() - pos);
        } else {
            path_ = url_;
        }
    }

    friend class detail::http_parser;

    bool https_{false};
    unsigned minor_version_{1};
    std::string_view url_;
    mutable std::string origin_;
    mutable std::string href_;
    std::string_view path_;
    std::string_view querystring_;
    mutable std::multimap<std::string, std::string> query_;
    std::string_view search_;
    std::string_view method_;
    std::uint64_t content_length_{0};
    bool websocket_{false};
    cookies& cookies_;
    std::string_view body_;
    std::vector<std::pair<std::string_view, std::string_view>> headers_;
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_REQUEST_HPP_

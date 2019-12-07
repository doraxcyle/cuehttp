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

#ifndef CUEHTTP_COMMON_HPP_
#define CUEHTTP_COMMON_HPP_

#include <string>
#include <algorithm>
#include <functional>
#include <chrono>
#include <map>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/3rd_party/http_parser.h"

namespace cue {
namespace http {

class context;

namespace detail {

// types
using reply_handler = std::function<bool(const std::string&)>;

// global variables
struct global_value final : safe_noncopyable {
    // for empty string const reference
    static const std::string& empty_string() noexcept {
        static const std::string empty{""};
        return empty;
    }

    static const std::string& cookie_expires_date() noexcept {
        static const std::string expires_date{"Thu, 01 Jan 1970 00:00:00 GMT"};
        return expires_date;
    }
};

// meta utilities
template <typename T>
using is_middleware = std::is_convertible<std::decay_t<T>, std::function<void(context&, std::function<void()>)>>;

template <typename T>
using is_middleware_without_next = std::is_convertible<std::decay_t<T>, std::function<void(context&)>>;

template <typename T>
using is_middleware_list =
    std::is_same<std::decay_t<T>, std::vector<std::function<void(context&, std::function<void()>)>>>;

// utilities functions
struct utils final : safe_noncopyable {
    inline static int64_t now() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    }

    inline static std::string to_method_string(unsigned method) noexcept {
        std::string method_string;
        switch (method) {
        case http_method::HTTP_DELETE:
            method_string = "DELETE";
            break;
        case http_method::HTTP_GET:
            method_string = "GET";
            break;
        case http_method::HTTP_HEAD:
            method_string = "HEAD";
            break;
        case http_method::HTTP_POST:
            method_string = "POST";
            break;
        case http_method::HTTP_PUT:
            method_string = "PUT";
            break;
        default:
            break;
        }
        return method_string;
    }

    inline static bool iequals(const std::string& lhs, const std::string& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                          [](char l, char r) { return std::tolower(l) == std::tolower(r); });
    }

    inline static std::string to_lower(const std::string& str) noexcept {
        std::string lower_str;
        for (const auto& c : str) {
            lower_str += std::tolower(c);
        }
        return lower_str;
    }

    static std::map<std::string, std::string> parse_query(const std::string& querystring) {
        static const auto decode = [](const std::string& str, std::string& decoded) {
            auto it = str.cbegin();
            auto end = str.cend();
            while (it != end) {
                char c{*it++};
                if (c == '%') {
                    if (it == end) {
                        return;
                    }
                    char hi{*it++};
                    if (it == end) {
                        return;
                    }
                    char lo{*it++};
                    if (hi >= '0' && hi <= '9') {
                        c = hi - '0';
                    } else if (hi >= 'A' && hi <= 'F') {
                        c = hi - 'A' + 10;
                    } else if (hi >= 'a' && hi <= 'f') {
                        c = hi - 'a' + 10;
                    } else {
                        return;
                    }
                    c *= 16;
                    if (lo >= '0' && lo <= '9') {
                        c += lo - '0';
                    } else if (lo >= 'A' && lo <= 'F') {
                        c += lo - 'A' + 10;
                    } else if (lo >= 'a' && lo <= 'f') {
                        c += lo - 'a' + 10;
                    } else {
                        return;
                    }
                }
                decoded += c;
            }
        };

        std::map<std::string, std::string> query;
        std::string::const_iterator it{querystring.begin()}, end{querystring.end()};
        while (it != end) {
            std::string name, value;
            while (it != end && *it != '=' && *it != '&') {
                if (*it == '+') {
                    name += ' ';
                } else {
                    name += *it;
                }
                ++it;
            }
            if (it != end && *it == '=') {
                ++it;
                while (it != end && *it != '&') {
                    if (*it == '+') {
                        value += ' ';
                    } else {
                        value += *it;
                    }
                    ++it;
                }
            }
            std::string decoded_name, decoded_value;
            decode(name, decoded_name);
            decode(value, decoded_value);
            query.emplace(std::move(decoded_name), std::move(decoded_value));
            if (it != end && *it == '&') {
                ++it;
            }
        }
        return query;
    }

    static std::string get_message_for_status(unsigned status) noexcept {
        switch (status) {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocols";
        case 102:
            return "Processing";
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 207:
            return "Multi-Status";
        case 208:
            return "Already Reported";
        case 226:
            return "IM Used";
        case 300:
            return "Multiple Choices";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 305:
            return "Use Proxy";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 402:
            return "Payment Required";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a Teapot";
        case 422:
            return "Unprocessable Entity";
        case 423:
            return "Locked";
        case 424:
            return "Failed Dependency";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 508:
            return "Loop Detected";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "???";
        }
    }

    inline static std::string to_gmt_string(std::time_t time) noexcept {
        struct tm* gmt = std::gmtime(&time);
        char buff[64]{0};
        std::strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S %Z", gmt);
        return buff;
    }

    static std::vector<std::string> split(const std::string& str, const std::string& separators) noexcept {
        size_t start{0};
        size_t end{str.find_first_of(separators)};
        std::vector<std::string> tookens;
        while (end <= std::string::npos) {
            tookens.emplace_back(str.substr(start, end - start));
            if (end == std::string::npos) {
                break;
            }
            start = end + 1;
            end = str.find_first_of(separators, start);
        }
        return tookens;
    }

    static std::string base64_encode(const std::string& source) {
        using base64_encode_iterator = boost::archive::iterators::base64_from_binary<
            boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>>;
        std::stringstream result;
        std::copy(base64_encode_iterator{source.cbegin()}, base64_encode_iterator{source.cend()},
                  std::ostream_iterator<char>{result});
        size_t equal_count{(3 - source.length() % 3) % 3};
        for (size_t i{0}; i < equal_count; ++i) {
            result.put('=');
        }
        return result.str();
    }

    static std::string base64_decode(const std::string& source) noexcept {
        using base64_decode_iterator = boost::archive::iterators::transform_width<
            boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6>;
        std::string result;
        try {
            std::string temp{source};
            size_t end_index{temp.size() - 1};
            while (temp[end_index] == '=') {
                temp.erase(end_index);
                end_index = temp.size() - 1;
            }

            std::stringstream stream_result;
            std::copy(base64_decode_iterator{temp.cbegin()}, base64_decode_iterator{temp.cend()},
                      std::ostream_iterator<char>{stream_result});
            result = stream_result.str();
        } catch (...) {
            return result;
        }
        return result;
    }

    inline static std::string uuid() {
        const auto uuid = boost::uuids::random_generator()();
        return boost::uuids::to_string(uuid);
    }
};

// utilities classes
class defer_ final : safe_noncopyable {
public:
    defer_() noexcept = default;

    ~defer_() noexcept {
        if (f_) {
            f_();
        }
    }

    defer_(defer_&& rhs) noexcept : f_{std::move(rhs.f_)} {
    }

    template <typename F>
    void operator+(F&& f) {
        f_ = std::forward<F>(f);
    }

private:
    std::function<void()> f_;
};

#define DEFER_CONCAT_NAME(l, r) l##r
#define DEFER_CREATE_NAME DEFER_CONCAT_NAME(__FUNCTION__, __LINE__)
#define defer                                      \
    cue::http::detail::defer_ DEFER_CREATE_NAME{}; \
    DEFER_CREATE_NAME +

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_COMMON_HPP_

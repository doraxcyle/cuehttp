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
#include <vector>
#include <string>
#include <random>
#if __cplusplus > 201402L
#include <string_view>
#else
#include "cuehttp/3rd_party/string_view.hpp"
namespace std {
using namespace nonstd;
namespace literals {
using namespace nonstd::literals::string_view_literals;
namespace string_view_literals {
using namespace nonstd::literals::string_view_literals;
} // namespace string_view_literals
} // namespace literals
namespace string_view_literals {
using namespace nonstd::literals::string_view_literals;
} // namespace string_view_literals
} // namespace std
#endif // __cplusplus > 201402L
#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/asio.hpp>
#ifdef ENABLE_HTTPS
#include <boost/asio/ssl.hpp>
#endif // ENABLE_HTTPS

#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/3rd_party/llhttp.h"

namespace cue {
namespace http {

class context;

namespace detail {

class http_parser;

struct ws_frame;

// types
using reply_handler = std::function<bool(const std::string&)>;
using http_socket = boost::asio::ip::tcp::socket;
#ifdef ENABLE_HTTPS
using https_socket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;
#endif // ENABLE_HTTPS
using ws_send_handler = std::function<void(detail::ws_frame&&)>;

enum class ws_opcode : std::uint8_t { continuation = 0, text = 1, binary = 2, close = 8, ping = 9, pong = 10 };

struct ws_reader final {
    char header[2]{0};
    std::vector<char> length_mask_buffer;
    bool fin{true};
    bool last_fin{true};
    ws_opcode opcode;
    bool has_mask{false};
    std::uint64_t length{0};
    char mask[4]{0};
    std::vector<char> payload_buffer;
};

struct ws_frame final {
    bool fin{true};
    ws_opcode opcode;
    bool mask{true};
    std::string payload;
};

// global variables
constexpr std::string_view cookie_expires_date{"Thu, 01 Jan 1970 00:00:00 GMT"};

constexpr std::string_view methods[] = {"DELETE", "GET", "HEAD", "POST", "PUT", "CONNECT", "OPTIONS"};

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
    static std::int64_t now() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    }

    static constexpr std::string_view to_method_string(unsigned method) noexcept {
        assert(method < sizeof(methods));
        return methods[method];
    }

    static bool iequals(std::string_view lhs, std::string_view rhs) noexcept {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                          [](char l, char r) { return std::tolower(l) == std::tolower(r); });
    }

    static std::string to_lower(std::string_view str) noexcept {
        std::string lower_str;
        for (const auto& c : str) {
            lower_str += std::tolower(c);
        }
        return lower_str;
    }

    static std::multimap<std::string, std::string> parse_query(std::string_view querystring) {
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

        std::multimap<std::string, std::string> query;
        std::string_view::const_iterator it{querystring.begin()}, end{querystring.end()};
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

    static std::string_view get_message_for_status(unsigned status) noexcept {
        using namespace std::literals;
        static std::unordered_map<unsigned, std::string_view> messages = {{100, "Continue"sv},
                                                                          {101, "Switching Protocols"sv},
                                                                          {102, "Processing"sv},
                                                                          {200, "OK"sv},
                                                                          {201, "Created"sv},
                                                                          {202, "Accepted"sv},
                                                                          {203, "Non-Authoritative Information"sv},
                                                                          {204, "No Content"sv},
                                                                          {205, "Reset Content"},
                                                                          {206, "Partial Content"sv},
                                                                          {207, "Multi-Status"sv},
                                                                          {208, "Already Reported"sv},
                                                                          {226, "IM Used"sv},
                                                                          {300, "Multiple Choices"sv},
                                                                          {301, "Moved Permanently"sv},
                                                                          {302, "Found"sv},
                                                                          {303, "See Other"sv},
                                                                          {304, "Not Modified"sv},
                                                                          {305, "Use Proxy"sv},
                                                                          {307, "Temporary Redirect"sv},
                                                                          {308, "Permanent Redirect"sv},
                                                                          {400, "Bad Request"sv},
                                                                          {401, "Unauthorized"sv},
                                                                          {402, "Payment Required"sv},
                                                                          {403, "Forbidden"sv},
                                                                          {404, "Not Found"sv},
                                                                          {405, "Method Not Allowed"sv},
                                                                          {406, "Not Acceptable"sv},
                                                                          {407, "Proxy Authentication Required"sv},
                                                                          {408, "Request Timeout"sv},
                                                                          {409, "Conflict"sv},
                                                                          {410, "Gone"sv},
                                                                          {411, "Length Required"sv},
                                                                          {412, "Precondition Failed"sv},
                                                                          {413, "Request Entity Too Large"sv},
                                                                          {414, "URI Too Long"sv},
                                                                          {415, "Unsupported Media Type"sv},
                                                                          {416, "Requested Range Not Satisfiable"sv},
                                                                          {417, "Expectation Failed"sv},
                                                                          {418, "I'm a Teapot"sv},
                                                                          {422, "Unprocessable Entity"sv},
                                                                          {423, "Locked"sv},
                                                                          {424, "Failed Dependency"sv},
                                                                          {426, "Upgrade Required"sv},
                                                                          {428, "Precondition Required"sv},
                                                                          {429, "Too Many Requests"sv},
                                                                          {431, "Request Header Fields Too Large"sv},
                                                                          {500, "Internal Server Error"sv},
                                                                          {501, "Not Implemented"sv},
                                                                          {502, "Bad Gateway"sv},
                                                                          {503, "Service Unavailable"sv},
                                                                          {504, "Gateway Timeout"sv},
                                                                          {505, "HTTP Version Not Supported"sv},
                                                                          {506, "Variant Also Negotiates"sv},
                                                                          {507, "Insufficient Storage"sv},
                                                                          {508, "Loop Detected"sv},
                                                                          {510, "Not Extended"sv},
                                                                          {511, "Network Authentication Required"sv}};
        return messages[status];
    }

    static std::string to_gmt_string(std::time_t time) noexcept {
        struct tm* gmt;
#ifdef __linux__
        struct tm now;
        gmt = gmtime_r(&time, &now);
#else
        gmt = gmtime(&time);
#endif // __linux__
        char buff[32]{0};
        std::strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S %Z", gmt);
        return buff;
    }

    static std::vector<std::string_view> split(std::string_view str, std::string_view separators) noexcept {
        std::size_t start{0};
        std::size_t end{str.find_first_of(separators)};
        std::vector<std::string_view> tookens;
        while (end <= std::string_view::npos) {
            tookens.emplace_back(str.substr(start, end - start));
            if (end == std::string_view::npos) {
                break;
            }
            start = end + 1;
            end = str.find_first_of(separators, start);
        }
        return tookens;
    }

    static std::string base64_encode(std::string_view source) {
        using base64_encode_iterator = boost::archive::iterators::base64_from_binary<
            boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8>>;
        std::stringstream result;
        std::copy(base64_encode_iterator{source.cbegin()}, base64_encode_iterator{source.cend()},
                  std::ostream_iterator<char>{result});
        std::size_t equal_count{(3 - source.length() % 3) % 3};
        for (std::size_t i{0}; i < equal_count; ++i) {
            result.put('=');
        }
        return result.str();
    }

    static std::string base64_decode(std::string_view source) noexcept {
        using base64_decode_iterator = boost::archive::iterators::transform_width<
            boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6>;
        std::string result;
        try {
            std::string temp{source};
            std::size_t end_index{temp.size() - 1};
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

    static std::string uuid() {
        const auto uuid = boost::uuids::random_generator()();
        return boost::uuids::to_string(uuid);
    }

    static std::uint32_t random_uint32() {
        std::mt19937 generator{static_cast<std::uint32_t>(std::chrono::system_clock::now().time_since_epoch().count())};
        return generator();
    }
};

// utilities classes

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_COMMON_HPP_

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

#ifndef CUEHTTP_CONTEXT_HPP_
#define CUEHTTP_CONTEXT_HPP_

#include <memory>

#include "cuehttp/request.hpp"
#include "cuehttp/response.hpp"
#include "cuehttp/cookies.hpp"
#include "cuehttp/session.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {

class context final : safe_noncopyable {
public:
    context(detail::reply_handler handler, bool https) noexcept
        : response_{cookies_, std::move(handler)}, request_{https, cookies_, response_} {
    }

    request& req() noexcept {
        return request_;
    }

    response& res() noexcept {
        return response_;
    }

    // request
    const std::map<std::string, std::string>& headers() const noexcept {
        return request_.headers();
    }

    const std::string& get(const std::string& field) const {
        return request_.get(field);
    }

    const std::string& method() const noexcept {
        return request_.method();
    }

    const std::string& host() const noexcept {
        return request_.host();
    }

    const std::string& hostname() const noexcept {
        return request_.hostname();
    }

    const std::string& url() const noexcept {
        return request_.url();
    }

    const std::string& origin() const noexcept {
        return request_.origin();
    }

    const std::string& href() const noexcept {
        return request_.href();
    }

    const std::string& path() const noexcept {
        return request_.path();
    }

    const std::string& querystring() const noexcept {
        return request_.querystring();
    }

    // response
    unsigned status() const noexcept {
        return response_.status();
    }

    void status(unsigned status) noexcept {
        response_.status(status);
    }

    template <typename Url>
    void redirect(Url&& url) {
        response_.redirect(std::forward<Url>(url));
    }

    template <typename Msg>
    void message(Msg&& message) {
        response_.message(std::forward<Msg>(message));
    }

    template <typename Field, typename Value>
    void set(Field&& field, Value&& value) {
        response_.set(std::forward<Field>(field), std::forward<Value>(value));
    }

    template <typename Headers>
    void set(Headers&& headers) {
        response_.set(std::forward<Headers>(headers));
    }

    void remove(const std::string& field) {
        response_.remove(field);
    }

    template <typename ContentType>
    void type(ContentType&& content_type) {
        response_.type(std::forward<ContentType>(content_type));
    }

    void length(long long content_length) noexcept {
        response_.length(content_length);
    }

    class cookies& cookies() {
        return cookies_;
    }

    bool session_enabled() const noexcept {
        return !!session_;
    }

    class session& session() noexcept {
        assert(session_);
        return *session_;
    }

    template <typename Options>
    void session(Options&& options) {
        session_ = std::make_shared<class session>(std::forward<Options>(options), *this, cookies_);
    }

    bool has_body() const noexcept {
        return response_.has_body();
    }

    template <typename Body>
    void body(Body&& body) {
        response_.body(std::forward<Body>(body));
    }

    void body(const char* buffer, size_t size) {
        response_.body(buffer, size);
    }

    std::ostream& body() {
        return response_.body();
    }

    void reset() noexcept {
        request_.reset();
        response_.reset();
        cookies_.reset();
    }

private:
    class cookies cookies_;
    response response_;
    request request_;
    std::shared_ptr<class session> session_{nullptr};
};

} // namespace http
} // namespace cue

#endif // CUEHTTP_CONTEXT_HPP_

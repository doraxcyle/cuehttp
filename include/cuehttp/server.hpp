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

#ifndef CUEHTTP_SERVER_HPP_
#define CUEHTTP_SERVER_HPP_

#include <thread>
#include <functional>
#include <vector>
#include <type_traits>

#include "cuehttp/deps/asio/asio.hpp"
#ifdef ENABLE_HTTPS
#include "cuehttp/deps/asio/asio/ssl.hpp"
#endif // ENABLE_HTTPS
#include "cuehttp/context.hpp"
#include "cuehttp/detail/connection.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/engines.hpp"

namespace cue {
namespace http {

template <typename _Socket, typename _Ty>
class base_server : safe_noncopyable {
public:
    base_server() noexcept {
    }

    explicit base_server(std::function<void(context&)> handler) noexcept : handler_{std::move(handler)} {
    }

    virtual ~base_server() = default;

    base_server(base_server&& rhs) noexcept {
        swap(rhs);
    }

    base_server& operator=(base_server&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    void swap(base_server& rhs) noexcept {
        if (this != std::addressof(rhs)) {
            std::swap(handler_, rhs.handler_);
            std::swap(acceptor_, rhs.acceptor_);
        }
    }

    base_server& listen(unsigned port) {
        assert(port != 0);
        listen_impl(asio::ip::tcp::resolver::query{std::to_string(port)});
        return *this;
    }

    template <typename _Host>
    base_server& listen(unsigned port, _Host&& host) {
        assert(port != 0);
        listen_impl(asio::ip::tcp::resolver::query{std::forward<_Host>(host), std::to_string(port)});
        return *this;
    }

    void run() {
        detail::engines::default_engines().run();
    }

protected:
    void listen_impl(asio::ip::tcp::resolver::query&& query) {
        auto& engines = detail::engines::default_engines();
        asio::ip::tcp::endpoint endpoint{*asio::ip::tcp::resolver{engines.get()}.resolve(query)};
        acceptor_ = std::make_unique<asio::ip::tcp::acceptor>(engines.get());
        acceptor_->open(endpoint.protocol());
        acceptor_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_->bind(endpoint);
        acceptor_->listen();
        do_accept();
    }

    void do_accept() {
        static_cast<_Ty&>(*this).do_accept_real();
    }

    std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
    std::function<void(context&)> handler_;
};

template <typename _Socket = detail::http_socket>
class server final : public base_server<_Socket, server<_Socket>>, safe_noncopyable {
public:
    server() noexcept = default;

    explicit server(std::function<void(context&)> handler) noexcept
        : base_server<_Socket, server<_Socket>>{std::move(handler)} {
    }

    server(server&& rhs) noexcept {
        swap(rhs);
    }

    server& operator=(server&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    void swap(server& rhs) noexcept {
        if (this != std::addressof(rhs)) {
            base_server<_Socket, server<_Socket>>::swap(rhs);
        }
    }

    void do_accept_real() {
        auto connector =
            std::make_shared<detail::connection<_Socket>>(this->handler_, detail::engines::default_engines().get());
        this->acceptor_->async_accept(connector->socket(), [this, connector](const std::error_code& code) {
            if (!code) {
                connector->socket().set_option(asio::ip::tcp::no_delay{true});
                connector->run();
            }

            this->do_accept();
        });
    }
};

#ifdef ENABLE_HTTPS
template <>
class server<detail::https_socket> final : public base_server<detail::https_socket, server<detail::https_socket>>,
                                           safe_noncopyable {
public:
    server(std::function<void(context&)> handler, const std::string& key, const std::string& cert) noexcept
        : base_server{std::move(handler)}, ssl_context_{asio::ssl::context::sslv23} {
        ssl_context_.use_certificate_chain_file(cert);
        ssl_context_.use_private_key_file(key, asio::ssl::context::pem);
    }

    server(server&& rhs) noexcept : ssl_context_{asio::ssl::context::sslv23} {
        swap(rhs);
    }

    server& operator=(server&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    void swap(server& rhs) noexcept {
        if (this != std::addressof(rhs)) {
            base_server::swap(rhs);
            std::swap(ssl_context_, rhs.ssl_context_);
        }
    }

    void do_accept_real() {
        auto connector = std::make_shared<detail::connection<detail::https_socket>>(
            this->handler_, detail::engines::default_engines().get(), ssl_context_);
        this->acceptor_->async_accept(connector->socket(), [this, connector](const std::error_code& code) {
            if (!code) {
                connector->socket().set_option(asio::ip::tcp::no_delay{true});
                connector->run();
            }

            this->do_accept();
        });
    }

private:
    asio::ssl::context ssl_context_;
};
#endif // ENABLE_HTTPS

using http_t = server<detail::http_socket>;

struct http final : safe_noncopyable {
    template <typename... _Args>
    static http_t create_server(_Args&&... args) noexcept {
        return http_t{std::forward<_Args>(args)...};
    }
};

#ifdef ENABLE_HTTPS
using https_t = server<detail::https_socket>;

struct https final : safe_noncopyable {
    template <typename... _Args>
    static https_t create_server(_Args&&... args) noexcept {
        return https_t{std::forward<_Args>(args)...};
    }
};
#endif // ENABLE_HTTPS

} // namespace http
} // namespace cue

#endif // CUEHTTP_SERVER_HPP_

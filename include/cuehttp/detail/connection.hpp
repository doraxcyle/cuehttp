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

#ifndef CUEHTTP_CONNECTION_HPP_
#define CUEHTTP_CONNECTION_HPP_

#include <memory>
#include <functional>
#include <boost/asio.hpp>

#include "cuehttp/context.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {
namespace detail {

template <typename Socket, typename T>
class base_connection : public std::enable_shared_from_this<base_connection<Socket, T>>, safe_noncopyable {
public:
    template <typename S = Socket, typename = std::enable_if_t<std::is_same<std::decay_t<S>, http_socket>::value>>
    base_connection(std::function<void(context&)> handler, boost::asio::io_service& io_service) noexcept
        : socket_{io_service},
          context_{std::bind(&base_connection::reply_chunk, this, std::placeholders::_1), false},
          handler_{std::move(handler)} {
    }

    virtual ~base_connection() noexcept = default;

#ifdef ENABLE_HTTPS
    template <typename S = Socket, typename = std::enable_if_t<!std::is_same<std::decay_t<S>, http_socket>::value>>
    base_connection(std::function<void(context&)> handler, boost::asio::io_service& io_service,
                    boost::asio::ssl::context& ssl_context) noexcept
        : socket_{io_service, ssl_context},
          context_{std::bind(&base_connection::reply_chunk, this, std::placeholders::_1), true},
          handler_{std::move(handler)} {
    }
#endif // ENABLE_HTTPS

    boost::asio::ip::tcp::socket& socket() noexcept {
        return static_cast<T&>(*this).socket();
    }

    void start() {
        do_read();
    }

protected:
    void do_read() {
        static_cast<T&>(*this).do_read_real();
    }

    void do_read_some() {
        socket_.async_read_some(
            boost::asio::buffer(context_.req().buffer()),
            [this, self = this->shared_from_this()](boost::system::error_code code, std::size_t bytes_transferred) {
                if (code) {
                    return;
                }

                const auto parsed = context_.req().parse(bytes_transferred);
                if (parsed != bytes_transferred || context_.req().has_parse_error()) {
                    reply_error(400);
                    return;
                }

                if (context_.req().has_more_requests()) {
                    do_read();
                    return;
                }

                handle_and_reply();
            });
    }

    void reply_error(unsigned status) {
        sync_headers();
        context_.status(status);
        reply();
    }

    void handle_and_reply() {
        handle();
        if (!context_.res().valid()) {
            return;
        }
        reply();
    }

    void handle() {
        sync_headers();
        assert(handler_);
        handler_(context_);
    }

    void sync_headers() {
        context_.res().version_major(context_.req().version_major());
        context_.res().version_minor(context_.req().version_minor());
        context_.res().keepalive(context_.req().keepalive());
    }

    void reply() {
        std::ostream os{&buffer_};
        os << context_.res();
        do_write();
    }

    void do_write() {
        boost::asio::async_write(
            socket_, buffer_,
            [this, self = this->shared_from_this()](boost::system::error_code code, std::size_t bytes_transferred) {
                if (code) {
                    return;
                }

                buffer_.consume(bytes_transferred);
                if (buffer_.size() != 0) {
                    do_write();
                    return;
                }

                check_keepalive();
            });
    }

    void check_keepalive() {
        if (context_.req().keepalive() && context_.res().keepalive()) {
            context_.reset();
            do_read();
        }
    }

    bool reply_chunk(const std::string& chunk) {
        boost::system::error_code code;
        boost::asio::write(socket_, boost::asio::buffer(chunk), code);
        return !!code;
    }

    Socket socket_;
    boost::asio::streambuf buffer_;
    context context_;
    std::function<void(context&)> handler_;
};

template <typename Socket = http_socket>
class connection final : public base_connection<Socket, connection<Socket>>, safe_noncopyable {
public:
    template <typename... Args>
    connection(Args&&... args) noexcept : base_connection<Socket, connection<Socket>>{std::forward<Args>(args)...} {
    }

    boost::asio::ip::tcp::socket& socket() noexcept {
        return this->socket_;
    }

    void do_read_real() {
        this->do_read_some();
    }
};

#ifdef ENABLE_HTTPS
template <>
class connection<https_socket> final : public base_connection<https_socket, connection<https_socket>>,
                                       safe_noncopyable {
public:
    template <typename... Args>
    connection(Args&&... args) noexcept : base_connection{std::forward<Args>(args)...} {
    }

    boost::asio::ip::tcp::socket& socket() noexcept {
        return socket_.next_layer();
    }

    void do_read_real() {
        if (has_handshake_) {
            do_read_some();
        } else {
            do_handshake();
        }
    }

private:
    void do_handshake() {
        socket_.async_handshake(boost::asio::ssl::stream_base::server,
                                [this, self = this->shared_from_this()](boost::system::error_code code) {
                                    if (code) {
                                        return;
                                    }

                                    has_handshake_ = true;
                                    do_read_some();
                                });
    }

    bool has_handshake_{false};
};
#endif // ENABLE_HTTPS

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_CONNECTION_HPP_

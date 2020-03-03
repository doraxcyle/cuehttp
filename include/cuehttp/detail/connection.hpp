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
#include <queue>
#include <mutex>
#include <boost/asio.hpp>

#include "cuehttp/context.hpp"
#include "cuehttp/detail/noncopyable.hpp"
#include "cuehttp/detail/endian.hpp"

namespace cue {
namespace http {
namespace detail {

template <typename Socket, typename T>
class base_connection : public std::enable_shared_from_this<base_connection<Socket, T>>, safe_noncopyable {
public:
    template <typename S = Socket, typename = std::enable_if_t<std::is_same<std::decay_t<S>, http_socket>::value>>
    base_connection(std::function<void(context&)> handler, boost::asio::io_service& io_service) noexcept
        : socket_{io_service},
          context_{std::bind(&base_connection::reply_chunk, this, std::placeholders::_1), false,
                   std::bind(&base_connection::send_ws_frame, this, std::placeholders::_1)},
          handler_{std::move(handler)} {
    }

    virtual ~base_connection() noexcept = default;

#ifdef ENABLE_HTTPS
    template <typename S = Socket, typename = std::enable_if_t<!std::is_same<std::decay_t<S>, http_socket>::value>>
    base_connection(std::function<void(context&)> handler, boost::asio::io_service& io_service,
                    boost::asio::ssl::context& ssl_context) noexcept
        : socket_{io_service, ssl_context},
          context_{std::bind(&base_connection::reply_chunk, this, std::placeholders::_1), true,
                   std::bind(&base_connection::send_ws_frame, this, std::placeholders::_1)},
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
    void close() {
        if (ws_handshake_) {
            context_.websocket().emit(detail::ws_event::close);
            ws_handshake_ = false;
        }
        boost::system::error_code code;
        socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, code);
        socket().close(code);
    }

    void do_read() {
        static_cast<T&>(*this).do_read_real();
    }

    void do_read_some() {
        socket_.async_read_some(
            boost::asio::buffer(context_.req().buffer()),
            [this, self = this->shared_from_this()](boost::system::error_code code, std::size_t bytes_transferred) {
                if (code) {
                    close();
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
                    close();
                    return;
                }

                buffer_.consume(bytes_transferred);
                if (buffer_.size() != 0) {
                    do_write();
                    return;
                }

                check_connection();
            });
    }

    void check_connection() {
        if (context_.req().websocket() && context_.status() == 101) {
            ws_handshake_ = true;
            if (!ws_reader_) {
                ws_reader_ = std::make_unique<detail::ws_reader>();
            }
            context_.websocket().emit(detail::ws_event::open);
            do_read_ws_header();
        } else {
            if (context_.req().keepalive() && context_.res().keepalive()) {
                context_.reset();
                do_read();
            }
        }
    }

    bool reply_chunk(const std::string& chunk) {
        boost::system::error_code code;
        boost::asio::write(socket_, boost::asio::buffer(chunk), code);
        return !!code;
    }

    void do_read_ws_header() {
        boost::asio::async_read(
            socket_, boost::asio::buffer(ws_reader_->header),
            [this, self = this->shared_from_this()](boost::system::error_code code, std::size_t bytes_transferred) {
                if (code) {
                    close();
                    return;
                }

                ws_reader_->fin = ws_reader_->header[0] & 0x80;
                ws_reader_->opcode = static_cast<detail::ws_opcode>(ws_reader_->header[0] & 0xf);
                ws_reader_->has_mask = ws_reader_->header[1] & 0x80;
                ws_reader_->length = ws_reader_->header[1] & 0x7f;
                if (ws_reader_->length == 126) {
                    do_read_ws_length_and_mask(2);
                } else if (ws_reader_->length == 127) {
                    do_read_ws_length_and_mask(8);
                } else {
                    do_read_ws_length_and_mask(0);
                }
            });
    }

    void do_read_ws_length_and_mask(std::size_t bytes) {
        const auto length = bytes + (ws_reader_->has_mask ? 4 : 0);
        if (length == 0) {
            handle_ws();
            return;
        }
        ws_reader_->length_mask_buffer.resize(length);
        boost::asio::async_read(
            socket_, boost::asio::buffer(ws_reader_->length_mask_buffer.data(), length),
            [bytes, this, self = this->shared_from_this()](boost::system::error_code code,
                                                           std::size_t bytes_transferred) {
                if (code) {
                    close();
                    return;
                }

                if (bytes == 2) {
                    ws_reader_->length =
                        detail::from_be(*reinterpret_cast<uint16_t*>(ws_reader_->length_mask_buffer.data()));
                    if (ws_reader_->has_mask) {
                        memcpy(ws_reader_->mask, ws_reader_->length_mask_buffer.data() + 2, 4);
                    }
                } else if (bytes == 8) {
                    ws_reader_->length =
                        detail::from_be(*reinterpret_cast<uint64_t*>(ws_reader_->length_mask_buffer.data()));
                    if (ws_reader_->has_mask) {
                        memcpy(ws_reader_->mask, ws_reader_->length_mask_buffer.data() + 8, 4);
                    }
                } else {
                    if (ws_reader_->has_mask) {
                        memcpy(ws_reader_->mask, ws_reader_->length_mask_buffer.data(), 4);
                    }
                }
                do_read_ws_payload();
            });
    }

    void do_read_ws_payload() {
        const std::size_t length{ws_reader_->payload_buffer.size()};
        ws_reader_->payload_buffer.resize(ws_reader_->length + length);
        boost::asio::async_read(
            socket_, boost::asio::buffer(ws_reader_->payload_buffer.data() + length, ws_reader_->length),
            [this, self = this->shared_from_this()](boost::system::error_code code, std::size_t bytes_transferred) {
                if (code) {
                    close();
                    return;
                }

                auto& payload_buffer = ws_reader_->payload_buffer;
                if (ws_reader_->has_mask) {
                    for (std::size_t i{0}; i < ws_reader_->length; ++i) {
                        payload_buffer[i] ^= ws_reader_->mask[i % 4];
                    }
                }
                handle_ws();
            });
    }

    void handle_ws() {
        switch (ws_reader_->opcode) {
        case detail::ws_opcode::continuation:
            ws_reader_->last_fin = false;
            break;
        case detail::ws_opcode::text:
        case detail::ws_opcode::binary: {
            if (ws_reader_->fin) {
                ws_reader_->last_fin = true;
                auto& payload_buffer = ws_reader_->payload_buffer;
                context_.websocket().emit(detail::ws_event::msg, {payload_buffer.data(), payload_buffer.size()});
                payload_buffer.clear();
            } else {
                ws_reader_->last_fin = false;
            }
            break;
        }
        case detail::ws_opcode::close:
            close();
            return;
        case detail::ws_opcode::ping:
            reply_ws_pong();
            break;
        default:
            break;
        }
        do_read_ws_header();
    }

    void reply_ws_pong() {
        detail::ws_frame frame;
        frame.opcode = detail::ws_opcode::pong;
        send_ws_frame(std::move(frame));
    }

    void send_ws_frame(detail::ws_frame&& frame) {
        std::unique_lock<std::mutex> lock{write_queue_mutex_};
        write_queue_.emplace(std::move(frame));

        if (write_queue_.size() == 1 && ws_handshake_) {
            lock.unlock();
            do_send_ws_frame();
        }
    }

    void do_send_ws_frame() {
        auto& frame = get_frame();
        std::ostream os{&buffer_};
        // opcode
        auto opcode = static_cast<uint8_t>(frame.opcode) | 0x80;
        os.write(reinterpret_cast<char*>(&opcode), 1);
        // length
        uint8_t base_length{0};
        uint16_t length16{0};
        uint64_t length64{0};
        const auto size = frame.payload.size();
        if (size < 126) {
            base_length |= static_cast<uint8_t>(size);
            os.write(reinterpret_cast<char*>(&base_length), sizeof(uint8_t));
        } else if (size <= UINT16_MAX) {
            base_length |= 0x7e;
            os.write(reinterpret_cast<char*>(&base_length), sizeof(uint8_t));
            length16 = detail::to_be(static_cast<uint16_t>(size));
            os.write(reinterpret_cast<char*>(&length16), sizeof(uint16_t));
        } else {
            base_length |= 0x7f;
            os.write(reinterpret_cast<char*>(&base_length), sizeof(uint8_t));
            length64 = detail::to_be(static_cast<uint64_t>(size));
            os.write(reinterpret_cast<char*>(&length64), sizeof(uint64_t));
        }

        if (size > 0) {
            // payload
            auto& payload = frame.payload;
            os.write(payload.data(), payload.size());
        }

        do_write_ws();
    }

    void do_write_ws() {
        boost::asio::async_write(
            socket_, buffer_,
            [this, self = this->shared_from_this()](boost::system::error_code code, std::size_t bytes_transferred) {
                if (code) {
                    close();
                    return;
                }

                buffer_.consume(bytes_transferred);
                if (buffer_.size() != 0) {
                    do_write_ws();
                    return;
                }

                std::unique_lock<std::mutex> lock{write_queue_mutex_};
                write_queue_.pop();
                if (!write_queue_.empty()) {
                    lock.unlock();
                    do_send_ws_frame();
                }
            });
    }

    detail::ws_frame& get_frame() {
        std::unique_lock<std::mutex> lock{write_queue_mutex_};
        assert(!write_queue_.empty());
        return write_queue_.front();
    }

    Socket socket_;
    boost::asio::streambuf buffer_;
    std::unique_ptr<detail::ws_reader> ws_reader_{nullptr};
    context context_;
    std::function<void(context&)> handler_;
    bool ws_handshake_{false};
    std::queue<detail::ws_frame> write_queue_;
    std::mutex write_queue_mutex_;
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
                                        close();
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

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

class connection final : public std::enable_shared_from_this<connection>, safe_noncopyable {
public:
    connection(boost::asio::io_service& io_service, std::function<void(context&)> handler) noexcept
        : socket_{io_service},
          context_{std::bind(&connection::reply_chunk, this, std::placeholders::_1)},
          handler_{std::move(handler)} {
    }

    boost::asio::ip::tcp::socket& socket() noexcept {
        return socket_;
    }

    void start() {
        do_read();
    }

private:
    void do_read() {
        const auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(context_.req().buffer()),
                                [this, self](boost::system::error_code code, std::size_t bytes_transferred) {
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
        context_.res().version(context_.req().version());
        context_.res().keepalive(context_.req().keepalive());
    }

    void reply() {
        std::ostream os{&buffer_};
        os << context_.res();
        do_write();
    }

    void do_write() {
        const auto self = shared_from_this();
        boost::asio::async_write(socket_, buffer_,
                                 [this, self](boost::system::error_code code, std::size_t bytes_transferred) {
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
        socket_.write_some(boost::asio::buffer(chunk), code);
        return !!code;
    }

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;
    context context_;
    std::function<void(context&)> handler_;
};

} // namespace detail
} // namespace http
} // namespace cue

#endif // CUEHTTP_CONNECTION_HPP_

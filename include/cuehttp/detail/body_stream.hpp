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

#ifndef CUEHTTP_BODY_STREAM_HPP_
#define CUEHTTP_BODY_STREAM_HPP_

#include "cuehttp/detail/buffered_streambuf.hpp"
#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {
namespace detail {

class body_streambuf final : public buffered_streambuf<>, safe_noncopyable {
 public:
  body_streambuf(bool chunked, reply_handler handler, std::ios::openmode mode) noexcept
      : buffered_streambuf{mode}, chunked_{chunked}, handler_{std::move(handler)}, openmode_{mode} {
    assert(handler_);
  }

  ~body_streambuf() override {
    if (openmode_ & std::ios::out) {
      sync();
      if (chunked_) {
        static const std::string chunked_end{"0\r\n\r\n"};
        handler_(chunked_end);
      }
    }
  }

  int read_from(char* buffer, std::streamsize size) final {
    detail::unused(buffer);
    detail::unused(size);
    return 0;
  }

  int write_to(const char* buffer, std::streamsize size) final {
    buffer_.clear();
    if (chunked_) {
      std::ostringstream stream;
      stream << std::hex << size;
      buffer_ += stream.str();
      static const std::string crlf{"\r\n"};
      buffer_ += crlf;
      buffer_.append(buffer, static_cast<std::string::size_type>(size));
      buffer_ += crlf;
    } else {
      buffer_.append(buffer, static_cast<std::string::size_type>(size));
    }
    handler_(buffer_);
    return static_cast<int>(size);
  }

 private:
  bool chunked_{true};
  reply_handler handler_;
  std::ios::openmode openmode_;
  std::string buffer_;
};

class body_ios : public virtual std::ios {
 public:
  body_ios(bool chunked, reply_handler handler, std::ios::openmode mode) noexcept
      : buffer_{chunked, std::move(handler), mode} {
    init(&buffer_);
  }

  body_streambuf* rdbuf() const { return const_cast<body_streambuf*>(&buffer_); }

 protected:
  body_streambuf buffer_;
};

class body_ostream final : public body_ios, public std::ostream, safe_noncopyable {
 public:
  body_ostream(bool chunked, reply_handler handler) noexcept
      : body_ios{chunked, std::move(handler), std::ios::out}, std::ostream{&buffer_} {}
};

}  // namespace detail
}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_BODY_STREAM_HPP_

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

#ifndef CUEHTTP_BUFFERED_STREAMBUF_HPP_
#define CUEHTTP_BUFFERED_STREAMBUF_HPP_

#include <array>
#include <streambuf>

#include "cuehttp/detail/common.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {
namespace detail {

template <std::size_t _BUFFER_SIZE = 1024 * 1024>
class buffered_streambuf : public std::streambuf, safe_noncopyable {
 public:
  explicit buffered_streambuf(std::ios::openmode mode) noexcept : mode_{mode} {
    setp(buffer_.begin(), buffer_.end());
    setg(buffer_.begin(), buffer_.begin(), buffer_.begin());
  }

  virtual ~buffered_streambuf() { sync(); }

  buffered_streambuf(buffered_streambuf&& rhs) noexcept { assgin_rv(std::move(rhs)); }

  buffered_streambuf& operator=(buffered_streambuf&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
      sync();
    }
    assgin_rv(std::move(rhs));
    return *this;
  }

  void swap(buffered_streambuf& rhs) noexcept {
    if (this != std::addressof(rhs)) {
      std::streambuf::swap(rhs);
      std::swap(mode_, rhs.mode_);
      buffer_.swap(rhs.buffer_);
    }
  }

  int_type overflow(int_type c = traits_type::eof()) final {
    if (!(mode_ & std::ios_base::out)) {
      return traits_type::eof();
    }

    if (flush() == traits_type::eof()) {
      return traits_type::eof();
    }

    if (traits_type::eq_int_type(c, traits_type::eof())) {
      return traits_type::not_eof(c);
    }

    *pptr() = traits_type::to_char_type(c);
    pbump(1);
    return c;
  }

  int_type underflow() final {
    if (!(mode_ & std::ios_base::in)) {
      return traits_type::eof();
    }

    const auto n = read_from(buffer_.begin(), _BUFFER_SIZE);
    if (n <= 0) {
      return traits_type::eof();
    }
    setg(buffer_.begin(), buffer_.begin(), buffer_.begin() + n);
    return traits_type::to_int_type(*pptr());
  }

  int sync() final {
    if (flush() == traits_type::eof()) {
      return traits_type::eof();
    }
    return 0;
  }

 private:
  void assign_rv(buffered_streambuf&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
      setp(nullptr, nullptr);
      setg(nullptr, nullptr, nullptr);
      swap(rhs);
    }
  }

  virtual int read_from(char* buffer, std::streamsize size) {
    detail::unused(buffer);
    detail::unused(size);
    return 0;
  }

  virtual int write_to(const char* buffer, std::streamsize size) {
    detail::unused(buffer);
    detail::unused(size);
    return 0;
  }

  int flush() {
    const auto n = pptr() - pbase();
    if (write_to(pbase(), n) == n) {
      pbump(-n);
      return n;
    }
    return traits_type::eof();
  }

  std::array<char, _BUFFER_SIZE> buffer_;
  std::ios::openmode mode_;
};

}  // namespace detail
}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_BUFFERED_STREAMBUF_HPP_

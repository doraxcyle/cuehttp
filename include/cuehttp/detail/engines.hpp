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

#ifndef CUEHTTP_ENGINES_HPP_
#define CUEHTTP_ENGINES_HPP_

#include <memory>
#include <vector>

#include "cuehttp/deps/asio/asio.hpp"
#include "cuehttp/detail/noncopyable.hpp"

namespace cue {
namespace http {
namespace detail {

class engines final : safe_noncopyable {
 public:
  explicit engines(std::size_t size) noexcept {
    assert(size != 0);
    for (std::size_t i{0}; i < size; ++i) {
      auto io_context = std::make_shared<asio::io_service>();
      auto worker = std::make_shared<asio::io_service::work>(*io_context);
      io_contexts_.emplace_back(std::move(io_context));
      workers_.emplace_back(std::move(worker));
    }
  }

  static engines& default_engines() noexcept {
    static engines engines{std::thread::hardware_concurrency()};
    return engines;
  }

  asio::io_service& get() noexcept { return *io_contexts_[index_++ % io_contexts_.size()]; }

  void run() {
    for (const auto& io_context : io_contexts_) {
      run_threads_.emplace_back([io_context]() { io_context->run(); });
    }

    for (auto& run_thread : run_threads_) {
      if (run_thread.joinable()) {
        run_thread.join();
      }
    }
  }

  void stop() {
    workers_.clear();
    for (const auto& io_context : io_contexts_) {
      io_context->stop();
    }
  }

 private:
  std::vector<std::shared_ptr<asio::io_service>> io_contexts_;
  std::vector<std::shared_ptr<asio::io_service::work>> workers_;
  std::vector<std::thread> run_threads_;
  std::size_t index_{0};
};

}  // namespace detail
}  // namespace http
}  // namespace cue

#endif  // CUEHTTP_ENGINES_HPP_

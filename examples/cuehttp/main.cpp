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

#include <iostream>
#include <vector>

#include "cuehttp.hpp"

using namespace cue::http;

void f1(context& ctx) { std::cout << "f1" << std::endl; }

void f2(context& ctx, std::function<void()> next) {
  std::cout << "f2" << std::endl;
  next();
}

struct handler1 {
  void handle(context& ctx) { std::cout << "handler1::handle" << std::endl; }
};

struct handler2 {
  void handle(context& ctx, std::function<void()> next) {
    std::cout << "handler2::handle" << std::endl;
    next();
  }
};

struct operator1 {
  void operator()(context& ctx) { std::cout << "operator1" << std::endl; }
};

struct operator2 {
  void operator()(context& ctx, std::function<void()> next) {
    std::cout << "operator2" << std::endl;
    next();
  }
};

int main(int argc, char** argv) {
  cuehttp app;
  app.use(f1);
  app.use(f2);

  handler1 hr1;
  app.use(&handler1::handle, &hr1);
  app.use(&handler1::handle);

  handler2 hr2;
  app.use(&handler2::handle, &hr2);
  app.use(&handler2::handle);

  operator1 or1;
  app.use(or1);

  operator2 or2;
  app.use(or2);

  app.use([](context& ctx) {
       ctx.type("text/html");
       ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
       ctx.status(200);
     })
      .use([](context& ctx, std::function<void()> next) {
        std::cout << "1-1" << std::endl;
        next();
        std::cout << "1-2" << std::endl;
      });

  const std::vector<std::function<void(context&, std::function<void()>)>> handlers{
      [](context& ctx, std::function<void()> next) {
        std::cout << "2-1" << std::endl;
        next();
        std::cout << "2-2" << std::endl;
      },
      [](context& ctx, std::function<void()> next) {
        std::cout << "3-1" << std::endl;
        std::cout << "3-2" << std::endl;
        next();
      }};
  app.use(std::move(handlers));

  app.use([](context& ctx) { std::cout << "4" << std::endl; });

#ifdef ENABLE_GZIP
  app.use(use_compress());
#endif  // ENABLE_GZIP

  app.listen(10001).run();
  // or
  // app.listen(10000);
  // cuehttp::run();

  // or
  // http::create_server(app.callback()).listen(10000).run();

  // or
  // auto http_server = http::create_server(app.callback());
  // http_server.listen(10000);
  // cuehttp::run();

  return 0;
}

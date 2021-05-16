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
  router route;
  route.get(
      "/get_multiple1",
      [](context& ctx, std::function<void()> next) {
        std::cout << "befor get" << std::endl;
        next();
      },
      [](context& ctx, std::function<void()> next) {
        std::cout << "handle get: " << ctx.path() << std::endl;
        ctx.type("text/html");
        ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
        next();
      },
      [](context& ctx, std::function<void()> next) { std::cout << "after get" << std::endl; });

  route.get(
      "/get_multiple2",
      [](context& ctx, std::function<void()> next) {
        std::cout << "befor get" << std::endl;
        next();
        std::cout << "after get" << std::endl;
      },
      [](context& ctx) {
        std::cout << "handle get: " << ctx.path() << std::endl;
        ctx.type("text/html");
        ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
      });

  handler1 hr1;
  handler2 hr2;
  route.get("/get_multiple3", &handler2::handle, &hr2, &handler2::handle,
            [](context& ctx) { std::cout << "after get" << std::endl; });

  route.get("/get_multiple4", &handler2::handle, &handler1::handle, &hr1,
            [](context& ctx) { std::cout << "after get" << std::endl; });

  route.get(
      "/get_multiple5", [](context& ctx) { std::cout << "befor get" << std::endl; },
      [](context& ctx) {
        std::cout << "handle get: " << ctx.path() << std::endl;
        ctx.type("text/html");
        ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
      },
      [](context& ctx) { std::cout << "after get" << std::endl; });

  route.post("/post", [](context& ctx, std::function<void()> next) {
    std::cout << "handle post: " << ctx.path() << std::endl;
    ctx.type("text/html");
    ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
  });

  route.get("/get1", [](context& ctx) {
    ctx.type("text/html");
    ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
  });

  route.get("/get2", f1);
  route.get("/get3", f2);

  handler1 h1;
  route.get("/get4", &handler1::handle, &h1);
  route.get("/get5", &handler1::handle);

  handler2 h2;
  route.get("/get6", &handler2::handle, &h2);
  route.get("/get7", &handler2::handle);

  operator1 o1;
  route.get("/get8", o1);

  operator1 o2;
  route.get("/get9", o2);

  cuehttp app;
  // app.use(route.routes());
  app.use(route);

  app.listen(10001).run();

  return 0;
}

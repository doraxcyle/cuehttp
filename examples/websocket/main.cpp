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

int main(int argc, char** argv) {
  cuehttp app;
  router route;
  app.ws().use(route.all("/get", [](context& ctx) {
    std::cout << "5" << std::endl;
    ctx.websocket().on_open([&ctx]() {
      std::cout << "websocket on_open" << std::endl;
      ctx.websocket().send("hello");
    });
    ctx.websocket().on_close([]() { std::cout << "websocket on_close" << std::endl; });
    ctx.websocket().on_message([&ctx](std::string&& msg) {
      std::cout << "websocket msg: " << msg << std::endl;
      ctx.websocket().send(std::move(msg));
    });
  }));

  router http_route;
  http_route.get("/get", [](context& ctx) {
    ctx.type("text/html");
    ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
    ctx.status(200);
  });
  app.use(http_route);

  auto http_server = http::create_server(app.callback());
  http_server.listen(10001);

#ifdef ENABLE_HTTPS
  auto https_server = https::create_server(app.callback(), "server.key", "server.crt");
  https_server.listen(443);
#endif  // #ifdef ENABLE_HTTPS

  std::cout << "websocket" << std::endl;

  std::thread{[&]() {
    for (;;) {
      // std::cout << "broadcast....." << std::endl;
      app.ws().broadcast("broadcast.....");
      std::this_thread::sleep_for(std::chrono::seconds{1});
    }
  }}.detach();

  cuehttp::run();

  return 0;
}

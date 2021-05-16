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

#include "cuehttp.hpp"

using namespace cue::http;

int main(int argc, char** argv) {
  router route;
  route.get("/test_session", [](context& ctx) {
    int view{1};
    const auto view_str = ctx.session().get("view");
    if (view_str.empty()) {
      ctx.session().set("view", std::to_string(view));
    } else {
      view = std::atoi(view_str.data());
      ctx.session().set("view", std::to_string(view + 1));
    }
    ctx.type("text/html");
    ctx.body(R"(<h1>Hello, )" + std::to_string(view) + R"( cuehttp!</h1>)");
    ctx.status(200);
  });

  session::options session_opt;
  session_opt.key = "cuehttp";
  // session_opt.external_key.get = [](context& ctx) {
  //     std::cout << "external_key.get" << std::endl;
  //     return ctx.get("User-Token");
  // };
  // session_opt.external_key.set = [](context& ctx, std::string_view value) {
  //     std::cout << "external_key.set" << std::endl;
  //     return ctx.set("User-Token", value);
  // };
  // session_opt.external_key.destroy = [](context& ctx, std::string_view value) {
  //     std::cout << "external_key.destroy" << std::endl;
  //     return ctx.remove("User-Token");
  // };
  cuehttp app;
  app.use(use_session(std::move(session_opt)));
  app.use(route.routes());

  app.listen(10001).run();

  return 0;
}

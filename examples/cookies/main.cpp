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

#include "cuehttp.hpp"

using namespace cue::http;

int main(int argc, char** argv) {
  router route;
  route.get("/cookie", [](context& ctx) {
    cookie::options options;
    options.domain = "127.0.0.1:10001";
    options.path = "/cookie";
    options.max_age = 60 * 60 * 24 * 1000;
    ctx.cookies().set("cue", "http", std::move(options));
    ctx.type("text/html");
    ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
    ctx.status(200);
  });

  cuehttp app;
  app.use(route.routes());

  app.listen(10001).run();

  return 0;
}

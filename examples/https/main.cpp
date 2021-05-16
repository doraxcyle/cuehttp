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
  cuehttp app;
  app.use([](context& ctx) {
    ctx.type("text/html");
    ctx.body(R"(<h1>Hello, cuehttp!</h1>)");
    ctx.status(200);
  });

  // both
  auto http_server = http::create_server(app.callback());
  http_server.listen(10001);

#ifdef ENABLE_HTTPS
  auto https_server = https::create_server(app.callback(), "server.key", "server.crt");
  https_server.listen(443);
#endif  // #ifdef ENABLE_HTTPS

  cuehttp::run();

  return 0;
}

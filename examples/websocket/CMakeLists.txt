cmake_minimum_required(VERSION 3.0)

project(websocket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-error=deprecated-declarations -Wno-deprecated-declarations")

set(SRC main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../../include)
include_directories(${PROJECT_SOURCE_DIR}/../../include/cuehttp/deps)
include_directories(${PROJECT_SOURCE_DIR}/../../include/cuehttp/deps/asio)

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

option(ENABLE_HTTPS "enable https" ON)
if (ENABLE_HTTPS)
  message("-- enable https")
  find_package(OpenSSL REQUIRED)
  include_directories(${OPENSSL_INCLUDE_DIR})
  add_definitions(-DENABLE_HTTPS)
endif ()

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
if (ENABLE_HTTPS)
  target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
endif ()

if (WIN32)
  target_link_libraries(${PROJECT_NAME} ws2_32 wsock32)
else ()
  target_link_libraries(${PROJECT_NAME} pthread)
endif ()

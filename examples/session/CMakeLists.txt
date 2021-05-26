cmake_minimum_required(VERSION 3.0)

project(session)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-error=deprecated-declarations -Wno-deprecated-declarations")

set(SRC main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../../include)
include_directories(${PROJECT_SOURCE_DIR}/../../include/cuehttp/deps)
include_directories(${PROJECT_SOURCE_DIR}/../../include/cuehttp/deps/asio)

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (WIN32)
  target_link_libraries(${PROJECT_NAME} ws2_32 wsock32 bcrypt)
else ()
  target_link_libraries(${PROJECT_NAME} pthread)
endif ()

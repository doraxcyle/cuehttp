cmake_minimum_required(VERSION 2.6)

project(session)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

set(SRC main.cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/../../../include)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 wsock32 bcrypt)
else ()
    target_link_libraries(${PROJECT_NAME} pthread)
endif ()

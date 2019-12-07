cmake_minimum_required(VERSION 2.6)

project(send_file)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

set(SRC main.cpp)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/../../../include)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 wsock32)
else ()
    target_link_libraries(${PROJECT_NAME} pthread)
endif ()

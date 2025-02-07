cmake_minimum_required(VERSION 3.30)
project(orderbook)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)
add_executable(orderbook_run ${SOURCE_FILES})

include_directories(src)
add_subdirectory(src)

target_link_libraries(orderbook_run src)

add_subdirectory(tests)
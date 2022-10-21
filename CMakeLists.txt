cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 17)              # Compile as C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Require C++17 support

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(server src/main.cpp src/buffer.cpp)

target_link_libraries(server -pthread)
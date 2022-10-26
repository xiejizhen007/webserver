cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 17)              # Compile as C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Require C++17 support

# output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# include directory
set(SERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SERVER_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test)
include_directories(${SERVER_SRC_INCLUDE_DIR} ${SERVER_TEST_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)

file(GLOB_RECURSE REMOVE_TARGET ${CMAKE_BINARY_DIR}/*)
add_custom_target(clean-all COMMAND ${CMAKE_COMMAND} -E remove ${REMOVE_TARGET})

# 
# add_executable(server src/main.cpp src/buffer.cpp)
# target_link_libraries(server -pthread)
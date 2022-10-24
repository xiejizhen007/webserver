# This is hacky :(
file(GLOB_RECURSE server_sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

# add_library(server_shared SHARED ${server_sources})
add_executable(server main.cpp ${server_sources})
target_link_libraries(server -pthread)
cmake_minimum_required(VERSION 3.20)
project(neural-experiments)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-O3 -fsanitize=address")
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(neural-experiments ${files})
target_link_libraries(neural-experiments sfml-graphics sfml-audio)
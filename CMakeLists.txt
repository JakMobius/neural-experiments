cmake_minimum_required(VERSION 3.20)
project(neural-experiments)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-O0 -fsanitize=address")
file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
add_executable(neural-experiments ${files})
target_link_libraries(neural-experiments sfml-graphics sfml-audio GLEW::GLEW OpenGL::GL)
include_directories(neural-experiments ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SFML_INCLUDE_DIRS})
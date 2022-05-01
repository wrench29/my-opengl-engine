cmake_minimum_required(VERSION 3.0.0)
project(opengl-engine VERSION 0.1.0)

file(GLOB SOURCES src/*.cpp)

add_executable(opengl-engine ${SOURCES})

target_link_libraries(opengl-engine glfw epoxy boost_system)


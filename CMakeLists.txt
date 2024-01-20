cmake_minimum_required(VERSION 3.15)
project(learning-opengl)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(main src/main.cpp)
target_link_libraries(main glfw )

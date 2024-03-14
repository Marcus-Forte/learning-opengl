cmake_minimum_required(VERSION 3.15)
project(learning-opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_GRPC "use GRPC listener for objects" OFF)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
find_package(OpenGL REQUIRED) # openGL library
find_package(GLEW REQUIRED) # openGL <-> GPU function pointers
find_package(glfw3 REQUIRED) # window system
find_package(glm REQUIRED) # For algebra

add_executable(main
  src/main.cpp
  src/grid.cc
  lib/inputProcessor.cpp)
target_link_libraries(main OpenGL::GL glfw GLEW::GLEW)
target_include_directories(main PUBLIC lib ext)

add_definitions(-DTEST_CLOUD_DIR="${CMAKE_CURRENT_SOURCE_DIR}/clouds")
if(USE_GRPC)
  add_subdirectory(grpc)
  target_link_libraries(main grpc_listener)
  add_definitions(-DUSE_GRPC=ON)
endif()

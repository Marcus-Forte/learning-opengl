cmake_minimum_required(VERSION 3.15)
project(learning-opengl)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
find_package(OpenGL REQUIRED) # openGL library
find_package(GLEW REQUIRED) # openGL <-> GPU function pointers
find_package(glfw3 REQUIRED) # window system
find_package(glm REQUIRED) # For algebra

add_executable(main
    src/main.cpp
    lib/inputProcessor.cpp)
target_link_libraries(main OpenGL::GL glfw GLEW::GLEW)
target_include_directories(main PUBLIC lib ext)

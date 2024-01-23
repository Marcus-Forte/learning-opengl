cmake_minimum_required(VERSION 3.15)
project(learning-opengl)

find_package(OpenGL REQUIRED) # openGL library
find_package(GLEW REQUIRED) # openGL <-> GPU function pointers
find_package(glm REQUIRED) # For algebra

add_executable(main
    src/main.cpp
    lib/inputProcessor.cpp)
target_link_libraries(main OpenGL::GL glfw ${GLEW_LIBRARIES})
target_include_directories(main PUBLIC lib ext)

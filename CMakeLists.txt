cmake_minimum_required(VERSION 3.15)
project(learning-opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_GRPC_LISTENER "use GRPC listener for objects" OFF)
option(FORMAT "Format code" ON)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
find_package(OpenGL REQUIRED) # openGL library
find_package(GLEW REQUIRED) # openGL <-> GPU function pointers
find_package(glfw3 REQUIRED) # window system
find_package(glm REQUIRED) # For algebra

add_library(gl_lib
lib/renderer.cc
lib/inputProcessor.cc
lib/shaderProgram.cc)
target_link_libraries(gl_lib
  OpenGL::GL glfw GLEW::GLEW)

add_executable(app
  src/app.cc
  src/grid.cc)
target_link_libraries(app gl_lib)
target_include_directories(app PUBLIC lib ext)

add_definitions(-DTEST_CLOUD_DIR="${CMAKE_CURRENT_SOURCE_DIR}/clouds")

if( ${FORMAT} )
message(STATUS "using clang-format")
file(GLOB SOURCEFILES 
"lib/**/*.cc"
"lib/**/*.hh"
"grpc/*.cc"
"grpc/*.hh"
"src/*.cc"
"src/*.hh"
)
string (REPLACE ";" " " SOURCEFILES "${SOURCEFILES}")
# message(STATUS ${SOURCEFILES})
add_custom_target(format ALL
COMMAND sh -c "clang-format -i ${SOURCEFILES} --verbose")
endif()



# 
if(BUILD_GRPC_LISTENER)
  add_subdirectory(grpc)
  add_executable(grpc_app src/grpc_app.cc)
  target_link_libraries(grpc_app gl_lib grpc_listener)
  target_include_directories(grpc_app PUBLIC lib)
endif()

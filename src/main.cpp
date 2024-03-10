#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <camera.h>

#include <filesystem>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <thread>

#include "entities/Axis.hh"
#include "entities/Line.hh"
#include "entities/Points.hh"
#include "inputProcessor.h"
#include "layouts/lineAttribute.hpp"
#include "layouts/pointAttribute.hpp"
#include "pointsLoader.hpp"
#include "renderer.hpp"
#include "shaderLoader.hpp"
#include "shaderProgram.hpp"
#include "vertexObject.h"

#ifndef TEST_CLOUD_DIR
#error "Test cloud dir not defined!
#endif

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1024, 800, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetKeyCallback(window, InputProcessor::keyboardCallback);
  glfwSetCursorPosCallback(window, InputProcessor::mouseCallback);
  glfwSetMouseButtonCallback(window, InputProcessor::mouseButtonCallback);
  glfwSetFramebufferSizeCallback(window,
                                 [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    // Problem: glewInit failed, something is wrong.
    std::cerr << "glewInit failed: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }

  auto testCloud = std::filesystem::path(TEST_CLOUD_DIR) / "bunny.pcd";

  auto bunnyPoints = loadFile(testCloud.string());

  Camera camera;

  InputProcessor::setCamera(&camera);

  Renderer renderer(window, camera);

  entity::Axis axis(0, 0, 0);
  renderer.addEntity(&axis, "axis");

  entity::Points pointcloud(bunnyPoints);
  pointcloud.setPointSize(5.0);
  renderer.addEntity(&pointcloud, "cloud");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    renderer.render();
  }
  glfwTerminate();
  return 0;
}
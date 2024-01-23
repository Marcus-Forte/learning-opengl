#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <camera.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <thread>

#include "entities/Line.hpp"
#include "inputProcessor.h"
#include "layouts/lineAttribute.hpp"
#include "layouts/pointAttribute.hpp"
#include "pointsLoader.hpp"
#include "renderer.hpp"
#include "shaderLoader.hpp"
#include "shaderProgram.hpp"
#include "vertexObject.h"

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1024, 800, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
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
    // Problem: glewInit failed, something is seriously wrong.
    std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }

  auto glDataArray = loadFile("/home/marcus/workspace/clouds/all_registered.txt");
  VertexObject buffer;
  buffer.setData(glDataArray.data(), glDataArray.size(), sizeof(GLPointData));
  buffer.setLayout<GLPointData>(2,
                                {
                                    3,
                                    3,
                                },
                                {GL_FLOAT, GL_FLOAT});

  auto glDataArray2 = loadFile("/home/marcus/workspace/clouds/myscan.xyz");
  VertexObject buffer2;
  buffer2.setData(glDataArray2.data(), glDataArray2.size(), sizeof(GLPointData));
  buffer2.setLayout<GLPointData>(2,
                                 {
                                     3,
                                     3,
                                 },
                                 {GL_FLOAT, GL_FLOAT});

  Camera camera;

  shaderLoader vertex_shader("../shaders/vertex.glsl", shaderLoader::ShaderType::VERTEX);
  auto vertexShader = vertex_shader.compile();
  shaderLoader fragment_shader("../shaders/fragment.glsl", shaderLoader::ShaderType::FRAGMENT);
  auto fragmentShader = fragment_shader.compile();

  ShaderProgram program(vertexShader, fragmentShader);

  shaderLoader line_vertex_shader("../shaders/line_vertex.glsl", shaderLoader::ShaderType::VERTEX);
  auto lineVertexShader = line_vertex_shader.compile();

  ShaderProgram line_program(lineVertexShader, fragmentShader);

  float line_color[3] = {0.0, 1.0, 0.0};

  line_program.bindUniformData(
      "transform", [&camera]() { return glm::value_ptr(camera.getMVP()); }, ShaderProgram::GLMATRIX4);
  line_program.bindUniformData(
      "color", [&line_color]() { return line_color; }, ShaderProgram::GLVEC3);
  program.bindUniformData(
      "transform", [&camera]() { return glm::value_ptr(camera.getMVP()); }, ShaderProgram::GLMATRIX4);

  InputProcessor::setCamera(&camera);

  Renderer renderer(window, camera);

  entity::Line line_entity{0.0, 0.0, 0.0, 2.0, 2.0, 2.0};
  line_entity.setShaderProgram(&line_program);
  // Entity pointcloud(buffer, program);
  // Entity pointcloud2(buffer2, program);
  // pointcloud2.setShaderProgram(&program);

  // renderer.addEntity(pointcloud, "cloud");
  // renderer.addEntity(pointcloud2, "cloud2");

  renderer.addEntity(line_entity, "line0");
  // renderer.addEntity(line_entity2, "line1");

  // glLineWidth(8.0);

  // std::thread t([&renderer, &line_program] {
  //   float x = 1.0;
  //   entity::Line line{0.0, 0.0, 0.0, 2.0, 2.0, 2.0};
  //   while (true) {
  //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //     std::cout << "Add line\n";

  //     // line_entity.setShaderProgram(&line_program);
  //     // renderer.addEntity(line_entity, "line" + std::to_string(x));
  //     // x++;
  //       }
  // });

  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    renderer.render();

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  glfwTerminate();
  return 0;
}
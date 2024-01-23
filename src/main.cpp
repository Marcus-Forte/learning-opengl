#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <camera.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "inputProcessor.h"
#include "shaderLoader.hpp"
#include "vertexBuffer.h"

Camera camera;
float g_pt_size = 5;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    // Problem: glewInit failed, something is seriously wrong.
    std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }

  // GL Data
  // float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f,
  // 0.5f, 1.0f}; float colors[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
  // 0.0, 1.0};

  // float vertices_colors[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
  //                            0.0f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,
  //                            0.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f};

  float vertices_colors[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
                             0.0f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f};
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  VertexBufferLayout layout;
  layout.addAttribute(VertexBufferLayout::float_, 3);  // positions
  layout.addAttribute(VertexBufferLayout::float_, 3);  // colors

  VertexBuffer buffer;

  buffer.setData(vertices_colors, sizeof(vertices_colors));
  buffer.setLayout(layout);
  buffer.bind();

  shaderLoader vertex_shader("../shaders/vertex.glsl",
                             shaderLoader::ShaderType::VERTEX);
  auto vertexShader = vertex_shader.compile();
  shaderLoader fragment_shader("../shaders/fragment.glsl",
                               shaderLoader::ShaderType::FRAGMENT);
  auto fragmentShader = fragment_shader.compile();

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::PROGRAM_FAILED\n"
              << infoLog << std::endl;
  }
  glUseProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

  InputProcessor::setCamera(&camera);

  glfwSetKeyCallback(window, InputProcessor::keyboardCallback);
  glfwSetCursorPosCallback(window, InputProcessor::mouseCallback);
  glfwSetMouseButtonCallback(window, InputProcessor::mouseButtonCallback);

  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no
                             // need to bind it every time, but we'll do so to
                             // keep things a bit more organized
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                       glm::value_ptr(camera.getMVP()));
    glDrawArrays(GL_POINTS, 0, 3);
    glPointSize(g_pt_size);
    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
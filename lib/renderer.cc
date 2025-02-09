#include "renderer.hh"

Renderer::Renderer() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(1024, 800, "Mview", NULL, NULL);
  if (window_ == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  input_processor_ = InputProcessor::getInstance(&camera_);

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetKeyCallback(window_, InputProcessor::keyboardCallbackFW);
  glfwSetCursorPosCallback(window_, InputProcessor::mouseCallbackFW);
  glfwSetMouseButtonCallback(window_, InputProcessor::mouseButtonCallbackFW);
  glfwSetFramebufferSizeCallback(window_,
                                 [](GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); });
  // exit(0);
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    // Problem: glewInit failed, something is wrong.
    std::cerr << "glewInit failed: " << glewGetErrorString(err) << std::endl;
    exit(1);
  }
}

Renderer::~Renderer() { glfwTerminate(); }

void Renderer::renderLoop() {
  glEnable(GL_PROGRAM_POINT_SIZE);

  while (!glfwWindowShouldClose(window_)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (input_processor_->resetCalled()) {
      clearEntities();
    }

    for (const auto &callback : callbacks_) {
      callback();
    }

    for (const auto &entity : entities_) {
      // Apply camera transform to all entities

      int program_id = entity.second->getShaderProgram()->getId();
      glUseProgram(program_id);
      int uniform_location = glGetUniformLocation(program_id, "transform");
      glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(camera_.getMVP()));

      entity.second->draw();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <entity.hpp>
#include <unordered_map>

class Renderer {
 public:
  Renderer(GLFWwindow* window, const Camera& camera) : window_(window), camera_ref_(camera) {}

  void addEntity(const Entity& entity, const std::string& name) {
    if (entities_.find(name) == entities_.end()) entities_.emplace(name, entity);
  }

  void useShader(unsigned int shaderId) const {
    glUseProgram(shaderId);
    
  }
  void render() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPointSize(5.0);
    // glUniformMatrix4fv(transformLoc_, 1, GL_FALSE, glm::value_ptr(camera_ref_.getMVP()));

    for (const auto& entity : entities_) {
      entity.second.draw();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

 private:
  std::unordered_map<std::string, Entity> entities_;
  const Camera& camera_ref_;
  GLFWwindow* window_;
};
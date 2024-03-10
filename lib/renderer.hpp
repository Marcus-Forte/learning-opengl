#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "entity.hh"

class Renderer {
 public:
  Renderer(GLFWwindow* window, const Camera& camera) : window_(window), camera_ref_(camera) {}

  void addEntity(const Entity* entity, const std::string& name) {
    if (entities_.find(name) == entities_.end()) entities_.emplace(name, entity);
  }

  void removeEntity(const std::string& name) {
    if (entities_.find(name) != entities_.end()) entities_.erase(name);
  }

  // void useShader(unsigned int shaderId) const { glUseProgram(shaderId); }
  void render() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for (const auto& entity : entities_) {
      // Apply camera transform to all entities

      int program_id = entity.second->getShaderProgram()->getId();
      glUseProgram(program_id);
      int uniform_location = glGetUniformLocation(program_id, "transform");
      glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(camera_ref_.getMVP()));

      entity.second->draw();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

 private:
  std::unordered_map<std::string, const Entity*> entities_;
  const Camera& camera_ref_;
  GLFWwindow* window_;
};
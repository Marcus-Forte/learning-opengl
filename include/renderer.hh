#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unordered_map>

#include "camera.hh"
#include "entity.hh"
#include "inputProcessor.hh"

class Renderer {
 public:
  using renderCallback = std::function<void(void)>;
  Renderer();
  virtual ~Renderer();

  void addEntity(std::shared_ptr<const Entity> entity, const std::string &name) {
    // Overwrite if entity exists.
    if (entities_.find(name) != entities_.end()) {
      entities_.erase(name);
    }
    entities_.emplace(name, entity);
  }

  void removeEntity(const std::string &name) {
    if (entities_.find(name) != entities_.end()) entities_.erase(name);
  }

  void listEntities() const {
    for (const auto &entity : entities_) {
      std::cout << entity.first << std::endl;
    }
  }

  inline void clearEntities() { entities_.clear(); }

  /// @brief Adds functions to be called
  /// each render loop.
  /// @param callback
  void registerCallback(const renderCallback &callback) { callbacks_.push_back(callback); }

  void renderLoop();

  Camera &getCamera() { return camera_; }

 private:
  std::unordered_map<std::string, std::shared_ptr<const Entity>> entities_;
  Camera camera_;
  GLFWwindow *window_;
  std::vector<renderCallback> callbacks_;
  std::shared_ptr<InputProcessor> input_processor_;
};
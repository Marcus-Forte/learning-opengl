#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/matrix.hpp>

class Camera {
 public:
  Camera() {
    position_ = glm::vec3(-2.0f, 0.0f, 0.0f);
    cameraX_ = glm::vec3(1.0f, 0.0f, 0.0f);
    cameraZ_ = glm::vec3(0.0f, 0.0f, 1.0f);
    perspective_ = glm::perspective(60.0f, 1.0f, 0.1f, 100.0f);
  }

  void translateLocalX(float delta) { position_ += cameraX_ * delta; }

  void translateLocalY(float delta) {
    auto cameraY = glm::cross(cameraX_, cameraZ_);
    position_ += cameraY * delta;
  }

  void translateLocalZ(float delta) { position_ += cameraZ_ * delta; }

  /// @brief rotate cameraX around Y local
  /// @return
  void rotateLocalY(float delta) {}
  void rotateLocalZ(float delta) {}

  glm::mat4 getMVP() const {
    glm::mat4 lookAt = glm::lookAt(position_, position_ + cameraX_, cameraZ_);

    return perspective_ * lookAt;
  }

  glm::vec3 getPosition() const { return position_; }
  glm::vec3 getLocalX() const { return cameraX_; }

 private:
  glm::vec3 position_;
  glm::vec3 cameraX_;  // front
  glm::vec3 cameraZ_;  // up
  glm::mat4 perspective_;
};
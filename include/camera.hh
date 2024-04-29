#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>

class Camera {
 public:
  Camera();

  void reset();

  void translateLocalX(float delta);
  void translateLocalY(float delta);

  void translateLocalZ(float delta);

  /// @brief rotate cameraX around Y local in radians.
  void rotateLocalY(float delta);

  void rotateLocalZ(float delta);

  void rotateLocalX(float delta);

  const glm::mat4 &getMVP() const;

  glm::vec3 getPosition() const;
  glm::vec3 getLocalX() const;

  void setPosition(float x, float y, float z);

 private:
  void applyTransform3(glm::vec3 &vec3, const glm::mat4 &transform);

  void init();
  glm::vec3 position_;
  glm::vec3 cameraX_;  // front
  glm::vec3 cameraZ_;  // up
  glm::mat4 perspective_;

  mutable glm::mat4 MVP_;
};
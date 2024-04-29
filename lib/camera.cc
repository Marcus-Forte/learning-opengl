
#include "camera.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>

Camera::Camera() { init(); }

void Camera::reset() { init(); }

void Camera::translateLocalX(float delta) { position_ += cameraX_ * delta; }

void Camera::translateLocalY(float delta) {
  auto cameraY = glm::cross(cameraX_, cameraZ_);
  position_ += cameraY * delta;
}

void Camera::translateLocalZ(float delta) { position_ += cameraZ_ * delta; }

void Camera::rotateLocalY(float delta) {
  auto cameraY = glm::cross(cameraX_, cameraZ_);
  glm::mat4 rotation(1.0f);
  rotation = glm::rotate(rotation, delta, cameraY);

  applyTransform3(cameraX_, rotation);
  applyTransform3(cameraZ_, rotation);
}

void Camera::rotateLocalZ(float delta) {
  glm::mat4 rotation(1.0f);
  rotation = glm::rotate(rotation, delta, cameraZ_);
  applyTransform3(cameraX_, rotation);
}

void Camera::rotateLocalX(float delta) {
  glm::mat4 rotation(1.0f);
  rotation = glm::rotate(rotation, delta, cameraX_);
  applyTransform3(cameraZ_, rotation);
  // applyTransform3(cameraY_, rotation);
}

const glm::mat4 &Camera::getMVP() const {
  glm::mat4 lookAt = glm::lookAt(position_, position_ + cameraX_, cameraZ_);
  MVP_ = perspective_ * lookAt;

  return MVP_;
}

glm::vec3 Camera::getPosition() const { return position_; }
glm::vec3 Camera::getLocalX() const { return cameraX_; }

void Camera::setPosition(float x, float y, float z) { position_ = glm::vec3(x, y, z); }

void Camera::applyTransform3(glm::vec3 &vec3, const glm::mat4 &transform) {
  glm::vec4 vec4_rep = glm::vec4(vec3, 1.0f);
  vec4_rep = transform * vec4_rep;
  vec3 = glm::vec3(vec4_rep);
}

void Camera::init() {
  position_ = glm::vec3(-2.0f, 1.0f, 1.0f);
  cameraX_ = glm::vec3(1.0f, 0.0f, 0.0f);
  cameraZ_ = glm::vec3(0.0f, 0.0f, 1.0f);
  perspective_ = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);
}

#include "inputProcessor.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <stdexcept>

Camera* InputProcessor::camera_ref_ = nullptr;
bool InputProcessor::mouse_pressed_ = false;
bool InputProcessor::shift_pressed_ = false;

void InputProcessor::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (!camera_ref_) throw std::runtime_error("Camera object not set by inputProcessor.\n");

  float speed = 1.0;
  float factor = 0.1;

  if (mods == GLFW_MOD_SHIFT) {
    factor = 0.5;
    shift_pressed_ = true ;
    // std::cout << "SHIFT OK\n";
    } else {
    shift_pressed_ = false ;
    // std::cout << "SHIFT UNPRESSED \n";
    }


  float delta = speed * factor;

  switch (key) {
    case (GLFW_KEY_W):
      // camera_ref_->translate(0.0, 0.0, 0.1);
      camera_ref_->translateLocalX(delta);
      break;

    case (GLFW_KEY_S):
      camera_ref_->translateLocalX(-delta);
      break;

    case (GLFW_KEY_E):
      camera_ref_->translateLocalZ(delta);
      break;

    case (GLFW_KEY_Q):
      camera_ref_->translateLocalZ(-delta);
      break;

    case (GLFW_KEY_A):
      camera_ref_->translateLocalY(-delta);
      break;

    case (GLFW_KEY_D):
      camera_ref_->translateLocalY(delta);
      break;
  }

  std::cout << glm::to_string(camera_ref_->getPosition()) << std::endl;
}

void InputProcessor::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  if (!mouse_pressed_) return;
  static double last_xpos = 0;
  static double last_ypos = 0;

  // std::cout << "motion:" << xpos << "," << ypos << std::endl << std::flush;
  double delta[2];
  delta[0] = xpos - last_xpos;
  delta[1] = ypos - last_ypos;

  last_ypos = ypos;
  last_xpos = xpos;

  constexpr double max = 10;

  if (delta[0] > max) delta[0] = max;

  if (delta[0] < -max) delta[0] = -max;

  if (delta[1] > max) delta[1] = max;

  if (delta[1] < -max) delta[1] = -max;

  // TODO smooth
  if(!shift_pressed_) {
    camera_ref_->rotateLocalY(delta[1] * 0.005f);
    camera_ref_->rotateLocalZ(delta[0] * 0.005f);
  } else {
    camera_ref_->rotateLocalX(delta[0] * 0.005f);
  }
}

void InputProcessor::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    InputProcessor::mouse_pressed_ = true;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    InputProcessor::mouse_pressed_ = false;
  }
}
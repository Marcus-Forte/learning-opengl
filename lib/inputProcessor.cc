#include "inputProcessor.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <stdexcept>

std::shared_ptr<InputProcessor> InputProcessor::singleton_ = nullptr;

/* Static forwaders */
void InputProcessor::keyboardCallbackFW(GLFWwindow *window, int key, int scancode, int action, int mods) {
  singleton_->keyboardCallback(window, key, scancode, action, mods);
}

void InputProcessor::mouseCallbackFW(GLFWwindow *window, double xpos, double ypos) {
  singleton_->mouseCallback(window, xpos, ypos);
}

void InputProcessor::mouseButtonCallbackFW(GLFWwindow *window, int button, int action, int mods) {
  singleton_->mouseButtonCallback(window, button, action, mods);
}
/* */

void InputProcessor::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (!camera_ref_) throw std::runtime_error("Camera object not set by inputProcessor.\n");

  pressed_mod_key_ = mods;

  if (mods == GLFW_MOD_SHIFT) {
    shift_pressed_ = true;
  } else {
    shift_pressed_ = false;
  }

  if (action == GLFW_PRESS) {
    pressed_keymap_.insert(key);
  }

  if (action == GLFW_RELEASE) {
    pressed_keymap_.erase(key);
  }
  if (key == GLFW_KEY_R) camera_ref_->reset();

  if (key == GLFW_KEY_M) {
    reset_called_ = true;
  }
}

bool InputProcessor::resetCalled() {
  bool reset_latch = reset_called_;
  reset_called_ = false;
  return reset_latch;
}

void InputProcessor::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (!mouse_pressed_) return;
  static double last_xpos = 0;
  static double last_ypos = 0;

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

  double factor = 0.0025f;

  // TODO smooth
  if (!shift_pressed_) {
    camera_ref_->rotateLocalY(delta[1] * factor);
    camera_ref_->rotateLocalZ(delta[0] * factor);
  } else {
    camera_ref_->rotateLocalX(delta[0] * factor);
  }
}

void InputProcessor::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    InputProcessor::mouse_pressed_ = true;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    InputProcessor::mouse_pressed_ = false;
  }
}

void InputProcessor::moveCameraOnKey(int key, int mod) const {
  float speed = 0.1;
  float factor = 0.5;

  if (mod == GLFW_MOD_SHIFT)
    factor = 1.0;
  else if (mod == GLFW_MOD_ALT)
    factor = 0.1;

  float delta = speed * factor;
  switch (key) {
    case GLFW_KEY_W:
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

    default:
      return;
  }
}

// Specialized thread to process inputs
void InputProcessor::inputProcessorThread() {
  while (running_) {
    for (auto key : pressed_keymap_) {
      moveCameraOnKey(key, pressed_mod_key_);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}
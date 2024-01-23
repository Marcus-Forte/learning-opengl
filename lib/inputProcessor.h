#pragma once

#include "GLFW/glfw3.h"
#include "camera.h"

/// @brief Processes keyboard and mouse inputs
class InputProcessor {
 public:
  static void setCamera(Camera* camera) { camera_ref_ = camera; }
  static void keyboardCallback(GLFWwindow* window, int key, int scancode,
                               int action, int mods);
  static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
  static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                  int mods);

 private:
  static Camera* camera_ref_;
  static bool mouse_pressed_;
};
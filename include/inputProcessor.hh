#pragma once

#include <functional>
#include <iostream>
#include <thread>
#include <unordered_set>

#include "GLFW/glfw3.h"
#include "camera.hh"

/// @brief Processes keyboard and mouse inputs
class InputProcessor {
 public:
  /* Singleton forwaders */
  static void keyboardCallbackFW(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseCallbackFW(GLFWwindow *window, double xpos, double ypos);
  static void mouseButtonCallbackFW(GLFWwindow *window, int button, int action, int mods);
  /* */
  void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  void inputProcessorThread();

  static std::unique_ptr<InputProcessor> &getInstance(Camera *camera) {
    if (!singleton_) {
      singleton_ = std::unique_ptr<InputProcessor>(new InputProcessor(camera));
    }

    return singleton_;
  }

  ~InputProcessor() {
    running_ = false;
    thread_.join();
  }

 private:
  InputProcessor(Camera *camera) : camera_ref_(camera) {
    running_ = true;
    thread_ = std::thread(&InputProcessor::inputProcessorThread, this);
  }

  void moveCameraOnKey(int key, int mod = false) const;

  static std::unique_ptr<InputProcessor> singleton_;
  Camera *camera_ref_;
  bool mouse_pressed_ = false;
  bool shift_pressed_ = false;
  int pressed_mod_key_;

  std::thread thread_;
  bool running_;

  std::unordered_set<int> pressed_keymap_;
};
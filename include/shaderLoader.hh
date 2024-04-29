#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "GL/glew.h"
class shaderLoader {
 public:
  enum class ShaderType { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

  shaderLoader(const std::filesystem::path& path, ShaderType type) {
    if (!std::filesystem::is_regular_file(path))
      throw std::runtime_error("shader path is not a file: " + path.string());

    std::string source;
    std::ifstream file(path);

    if (!file.is_open()) throw std::runtime_error("Unable to open shader: " + path.string());

    std::string line;
    while (getline(file, line)) {
      source += line + "\n";
    }

    file.close();

    const char* vertexShaderSource = source.c_str();

    shaderId = glCreateShader((GLenum)type);
    glShaderSource(shaderId, 1, &vertexShaderSource, NULL);
  }

  /// @brief Compile shader program.
  /// @return shaderId
  unsigned int compile() const {
    int success;
    char infoLog[512];
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
      throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED\n" + std::string(infoLog));
    }
    return shaderId;
  }

 private:
  unsigned int shaderId;
};
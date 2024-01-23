#pragma once

#include <GL/glew.h>

class ShaderProgram {
 public:
  ShaderProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) {
    shader_program_id_ = glCreateProgram();
    glAttachShader(shader_program_id_, vertexShaderId);
    glAttachShader(shader_program_id_, fragmentShaderId);
    glLinkProgram(shader_program_id_);

    int success;
    char infoLog[512];
    glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader_program_id_, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::PROGRAM_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
  }

  virtual ~ShaderProgram() { glDeleteProgram(shader_program_id_); }

  void setDataFetchCb(std::function<const float*()> fun) { uniform_data_getter_ = fun; }

  void use() const {
    glUseProgram(shader_program_id_);
    unsigned int transformLoc = glGetUniformLocation(shader_program_id_, "transform");

    if (transformLoc == -1) throw std::runtime_error("Error assigning uniform data.");

    if (uniform_data_getter_ != nullptr) {
      auto data = uniform_data_getter_();
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, data);
    }
  }

  unsigned int getId() const { return shader_program_id_; }

 private:
  unsigned int shader_program_id_;
  std::function<const float*()> uniform_data_getter_ = nullptr;
};
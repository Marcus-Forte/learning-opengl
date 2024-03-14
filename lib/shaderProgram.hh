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
      std::cerr << "ERROR::SHADER::VERTEX::PROGRAM_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
  }

  virtual ~ShaderProgram() { glDeleteProgram(shader_program_id_); }

  void use() const { glUseProgram(shader_program_id_); }

  unsigned int getId() const { return shader_program_id_; }

 private:
  unsigned int shader_program_id_;
};
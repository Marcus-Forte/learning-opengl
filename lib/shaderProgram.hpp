#pragma once

#include <GL/glew.h>

class ShaderProgram {
 public:
  enum glUniformSetter {
    GLMATRIX4,
    GLVEC3,
  };

  using uniforDataSetter = std::function<const float*()>;
  using uniformFunStruct = std::tuple<std::string, uniforDataSetter, glUniformSetter>;

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

  void bindUniformData(const std::string& uniform_name, uniforDataSetter data, glUniformSetter uniformSetter) {
    uniform_binders_.push_back({uniform_name, data, uniformSetter});
  }

  void use() const {
    glUseProgram(shader_program_id_);

    for (const auto& uniform : uniform_binders_) {
      auto uniform_name = std::get<0>(uniform);
      auto uniform_data_fetcher = std::get<1>(uniform);
      auto gl_uniform_setter = std::get<2>(uniform);
      int uniform_location = glGetUniformLocation(shader_program_id_, uniform_name.c_str());
      if (uniform_location == -1) throw std::runtime_error("Error assigning uniform data: `" + uniform_name + "`");
      auto uniform_data = uniform_data_fetcher();
      if (gl_uniform_setter == GLMATRIX4) {
        glUniformMatrix4fv(uniform_location, 1, GL_FALSE, uniform_data);
      } else if (gl_uniform_setter == GLVEC3) {
        glUniform3f(uniform_location, uniform_data[0], uniform_data[1], uniform_data[2]);
      } else {
        throw std::runtime_error("Unknown GL Uniform setter");
      }
    }
  }

  unsigned int getId() const { return shader_program_id_; }

 private:
  unsigned int shader_program_id_;
  std::vector<uniformFunStruct> uniform_binders_;

  // shader_program_uniforms_;
};
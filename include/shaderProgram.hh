#pragma once

#include <GL/glew.h>

#include <filesystem>

class ShaderProgram {
 public:
  ShaderProgram(const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path);

  virtual ~ShaderProgram() { glDeleteProgram(shader_program_id_); }

  inline void use() const { glUseProgram(shader_program_id_); }

  inline unsigned int getId() const { return shader_program_id_; }

 private:
  unsigned int shader_program_id_;
};
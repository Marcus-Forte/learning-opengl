#pragma once

#include <memory>

#include "shaderLoader.hh"
#include "shaderProgram.hh"
#include "vertexObject.h"
class Entity {
 public:
  Entity(const std::shared_ptr<ShaderProgram>& shader) : shader_program_(shader) {}

  std::shared_ptr<ShaderProgram> getShaderProgram() const { return shader_program_; }
  virtual void draw() const {
    if (!shader_program_) throw std::runtime_error("No shader program set for entity.");
    vertex_object_.bind();
    shader_program_->use();
    glDrawArrays(gl_draw_type_, 0, vertex_object_.getNumVertices());
    vertex_object_.unbind();
  }

 protected:
  inline void setGlDrawType(unsigned int gl_draw_type) {
    if (gl_draw_type != GL_POINTS && gl_draw_type != GL_LINES) throw std::runtime_error("Invalid GL Draw type.");
    gl_draw_type_ = gl_draw_type;
  }
  VertexObject vertex_object_;
  std::shared_ptr<ShaderProgram> shader_program_;
  unsigned int gl_draw_type_ = GL_POINTS;
  bool internal_program = false;
};
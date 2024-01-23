#pragma once

#include "shaderProgram.hpp"
#include "vertexObject.h"
class Entity {
 public:
  Entity(const VertexObject& buffer_object, const ShaderProgram& shader_program)
      : vertex_object_(buffer_object), shader_program_(shader_program) {}
  virtual ~Entity() = default;

  virtual void draw() const {
    vertex_object_.bind();
    shader_program_.use();
    glDrawArrays(gl_draw_type_, 0, vertex_object_.getNumVertices());
    vertex_object_.unbind();
  }

  inline void setGlDrawType(unsigned int gl_draw_type) {
    if (gl_draw_type != GL_POINTS && gl_draw_type != GL_LINES) throw std::runtime_error("Invalid GL Draw type.");
    gl_draw_type_ = gl_draw_type;
  }

 private:
  VertexObject vertex_object_;
  ShaderProgram shader_program_;
  unsigned int gl_draw_type_ = GL_POINTS;
};
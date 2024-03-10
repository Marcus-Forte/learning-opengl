#pragma once

#include "shaderLoader.hpp"
#include "shaderProgram.hpp"
#include "vertexObject.h"
class Entity {
 public:
  Entity() {
    // use basic shader
    shaderLoader vertex_shader("../shaders/vertex.glsl", shaderLoader::ShaderType::VERTEX);
    auto vertexShader = vertex_shader.compile();
    shaderLoader fragment_shader("../shaders/fragment.glsl", shaderLoader::ShaderType::FRAGMENT);
    auto fragmentShader = fragment_shader.compile();
    shader_program_ = new ShaderProgram(vertexShader, fragmentShader);
    internal_program = true;
  }
  // Entity(const VertexObject& buffer_object) : vertex_object_(buffer_object) { Entity(); }

  virtual ~Entity() {
    if (internal_program) delete shader_program_;
  }

  const ShaderProgram* getShaderProgram() const { return shader_program_; }
  virtual void draw() const {
    if (!shader_program_) throw std::runtime_error("No shader program set for entity.");
    vertex_object_.bind();
    shader_program_->use();
    glDrawArrays(gl_draw_type_, 0, vertex_object_.getNumVertices());
    vertex_object_.unbind();
  }

  inline void setGlDrawType(unsigned int gl_draw_type) {
    if (gl_draw_type != GL_POINTS && gl_draw_type != GL_LINES) throw std::runtime_error("Invalid GL Draw type.");
    gl_draw_type_ = gl_draw_type;
  }

 protected:
  VertexObject vertex_object_;
  const ShaderProgram* shader_program_ = nullptr;
  unsigned int gl_draw_type_ = GL_POINTS;
  bool internal_program = false;
};
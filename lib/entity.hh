#pragma once

#include "shaderLoader.hh"
#include "shaderProgram.hh"
#include "vertexObject.h"
class Entity {
 public:
  Entity() {
    // use basic shader
    shaderLoader vertex_shader("../shaders/point-vert.glsl", shaderLoader::ShaderType::VERTEX);
    auto vertexShader = vertex_shader.compile();
    shaderLoader fragment_shader("../shaders/fragment.glsl", shaderLoader::ShaderType::FRAGMENT);
    auto fragmentShader = fragment_shader.compile();
    shader_program_ = std::unique_ptr<ShaderProgram>(new ShaderProgram(vertexShader, fragmentShader));
    internal_program = true;
  }

  // TODO have renderer call it as friend?
  const std::unique_ptr<ShaderProgram> &getShaderProgram() const { return shader_program_; }
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
  std::unique_ptr<ShaderProgram> shader_program_ = nullptr;
  unsigned int gl_draw_type_ = GL_POINTS;
  bool internal_program = false;
};
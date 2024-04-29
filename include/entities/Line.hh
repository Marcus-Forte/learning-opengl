#pragma once

#include <memory>

#include "../entity.hh"
#include "../layouts/lineAttribute.hpp"

namespace entity {
class Line : public Entity {
 public:
  Line(float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z, float r = 1.0, float g = 0.0,
       float b = 0.0)
      : Entity(std::make_shared<ShaderProgram>("../shaders/line-vert.glsl", "../shaders/fragment.glsl")),
        p0_{p0_x, p0_y, p0_z},
        p1_{p1_x, p1_y, p1_z},
        color_{r, g, b} {
    GLLineData line_data{p0_[0], p0_[1], p0_[2], p1_[0], p1_[1], p1_[2]};
    VertexBufferLayout layout_;
    layout_.addAttribute(VertexBufferLayout::float_, 3);  // pos
    vertex_object_.setLayout(layout_);
    vertex_object_.setData(&line_data, 2, sizeof(float) * 3);

    // Add color
    int program_id = shader_program_->getId();
    glUseProgram(program_id);
    int uniform_location = glGetUniformLocation(program_id, "color");
    if (uniform_location == -1) {
      throw std::runtime_error("uniform not found in Line entity.");
    }
    glUniform3f(uniform_location, color_[0], color_[1], color_[2]);

    gl_draw_type_ = GL_LINES;
  }

  void setWidth(float width) { line_width_ = width; }

 private:
  void draw() const override {
    glLineWidth(line_width_);
    Entity::draw();
  }

  const float p0_[3];
  const float p1_[3];
  const float color_[3];
  float line_width_ = 5.0f;
};
}  // namespace entity
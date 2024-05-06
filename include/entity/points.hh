#pragma once

#include <memory>

#include "../entity.hh"
#include "../layouts/pointAttribute.hpp"
#include "shaderProgram.hh"

namespace entity {
class Points : public Entity {
 public:
  Points(const std::shared_ptr<ShaderProgram>& program, const std::vector<GLPointData>& points )
      : Entity(program) {
    vertex_object_.setLayout<GLPointData>(2, {3, 3}, {GL_FLOAT, GL_FLOAT});
    vertex_object_.setData(points.data(), points.size(), sizeof(GLPointData));
  }

  void draw() const override {
    int program_id = shader_program_->getId();
    int uniform_location = glGetUniformLocation(program_id, "point_size");
    glUniform1f(uniform_location, point_size_);
    Entity::draw();
  }

  void setPointSize(float size) { point_size_ = size; }

 private:
  float point_size_ = 1.0;
};
}  // namespace entity
#pragma once

#include "entity.hh"
#include "layouts/pointAttribute.hpp"

namespace entity {
class Points : public Entity {
 public:
  Points(std::vector<GLPointData> points) {
    vertex_object_.setData(points.data(), points.size(), sizeof(GLPointData));
    vertex_object_.setLayout<GLPointData>(2, {3, 3}, {GL_FLOAT, GL_FLOAT});
  }

  void draw() const override {
    glPointSize(point_size_);
    Entity::draw();
  }

  void setPointSize(float size) { point_size_ = size; }

 private:
  float point_size_ = 1.0;
  ;
};
}  // namespace entity
#include "grid.hh"

#include <thread>

#include "entities/Line.hh"
void add_grid(Renderer *renderer) {
  static const float x_ = 5.0f;
  static const float y_ = 5.0f;
  float step_ = 0.5;
  for (float s = 0.0; s <= 10.0; s += step_) {
    float x0 = x_;
    float y0 = y_ - s;
    float x1 = -x0;
    float y1 = y0;
    std::shared_ptr<entity::Line> lx(new entity::Line(x0, y0, 0, x1, y1, 0, 0.0, 1.0, 0.0));
    lx->setWidth(2.0f);
    renderer->addEntity(lx, "lx" + std::to_string(s));

    std::shared_ptr<entity::Line> ly(new entity::Line(y0, x0, 0, y1, x1, 0, 0.0, 1.0, 0.0));
    ly->setWidth(2.0f);
    renderer->addEntity(ly, "ly" + std::to_string(s));
  }
}


#include <filesystem>
#include <iostream>

#include "entities/Axis.hh"
#include "entities/Line.hh"
#include "entities/Points.hh"
#include "layouts/lineAttribute.hpp"
#include "layouts/pointAttribute.hpp"
#include "pointsLoader.hpp"
#include "renderer.hh"

#ifndef TEST_CLOUD_DIR
#error "Test cloud dir not defined!
#endif

#include "grid.hh"

int main(int argc, char **argv) {
  auto testCloud = std::filesystem::path(TEST_CLOUD_DIR) / "bunny.pcd";

  auto loaded_points = loadFile(testCloud.string());

  Renderer renderer;

  std::shared_ptr<entity::Points> pointcloud(new entity::Points(loaded_points));
  pointcloud->setPointSize(5.0);
  renderer.addEntity(pointcloud, "cloud");

  auto testCloud2 = std::filesystem::path(TEST_CLOUD_DIR) / "cat.pcd";
  auto loaded_points2 = loadFile(testCloud2.string());
  std::shared_ptr<entity::Points> pointcloud2(new entity::Points(loaded_points2));
  pointcloud2->setPointSize(15.0);
  renderer.addEntity(pointcloud2, "cloud2");

  add_grid(&renderer);
  std::shared_ptr<entity::Axis> axis(new entity::Axis(0, 0, 0));
  renderer.addEntity(axis, "axis");

  renderer.renderLoop();

  return 0;
}
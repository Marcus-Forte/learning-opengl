

#include <filesystem>
#include <iostream>

#include "entity/factory.hh"
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
  entity::EntityFactory factory;

  auto pointcloud = factory.create_points(loaded_points);
  pointcloud->setPointSize(5.0);
  renderer.addEntity(pointcloud, "cloud");

  auto testCloud2 = std::filesystem::path(TEST_CLOUD_DIR) / "cat.pcd";
  auto loaded_points2 = loadFile(testCloud2.string());
  auto pointcloud2 = factory.create_points(loaded_points2);
  pointcloud2->setPointSize(15.0);
  renderer.addEntity(pointcloud2, "cloud2");

  add_grid(&renderer);
  auto axis = factory.create_axis(0, 0, 0);
  renderer.addEntity(axis, "axis");

  renderer.renderLoop();

  return 0;
}
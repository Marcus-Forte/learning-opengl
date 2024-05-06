

#include <filesystem>
#include <iostream>

#include "grpc_conv.hh"
#include "grpc_listener.hh"
#include "layouts/lineAttribute.hpp"
#include "layouts/pointAttribute.hpp"
#include "renderer.hh"

int main(int argc, char **argv) {
  Renderer renderer;
  auto &camera = renderer.getCamera();
  auto entity_factory = std::make_shared<entity::EntityFactory>();

  camera.rotateLocalY(-1.57);
  camera.setPosition(0, 0, 20.0);

  // std::shared_ptr<entity::Axis> axis(new entity::Axis(0, 0, 0));
  // renderer.addEntity(axis, "axis");

  grpc_listener::SharedQueue shared_queue;
  grpc_listener::gRPCListener listener(shared_queue);
  listener.startAsync();

  renderer.registerCallback([&] { processgRPCQueue(shared_queue, renderer, entity_factory); });

  renderer.renderLoop();

  listener.shutDown();

  return 0;
}



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

  camera.setPosition(-3.0, 1.0, 1.0);

  grpc_listener::SharedQueue shared_queue;
  shared_queue.reset_scene = true;

  grpc_listener::gRPCListener listener(shared_queue);
  listener.startAsync();

  renderer.registerCallback([&] { processgRPCQueue(shared_queue, renderer, entity_factory); });

  renderer.renderLoop();

  listener.shutDown();

  return 0;
}

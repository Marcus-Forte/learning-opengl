

#include <filesystem>
#include <iostream>

#include "entities/axis.hh"
#include "entities/line.hh"
#include "entities/points.hh"
#include "grid.hh"
#include "grpc_conv.hh"
#include "grpc_listener.hh"
#include "layouts/lineAttribute.hpp"
#include "layouts/pointAttribute.hpp"
#include "renderer.hh"

int main(int argc, char** argv) {
  Renderer renderer;

  std::shared_ptr<entity::Axis> axis(new entity::Axis(0, 0, 0));
  renderer.addEntity(axis, "axis");

  grpc_listener::SharedQueue shared_queue;
  grpc_listener::gRPCListener listener(shared_queue);
  listener.startAsync();

  renderer.registerCallback([&] { processgRPCQueue(shared_queue, renderer); });

  renderer.renderLoop();

  listener.shutDown();

  return 0;
}

#pragma once

#include "entity/factory.hh"
#include "grpc_listener.hh"
#include "renderer.hh"

void processgRPCQueue(grpc_listener::SharedQueue &shared_queue, Renderer &renderer,
                      const std::shared_ptr<entity::EntityFactory> &factory);
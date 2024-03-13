#pragma once
#include "entities/points.hh"
#include "grpc_listener.hh"
#include "renderer.hpp"
/* GRPC conversion utilities. */
void gRPCtoGL(SharedQueue& shared_queue, Renderer& renderer) {
  if (shared_queue.size()) {
    static int s_grpc_entity_count = 0;
    // Extract points
    std::vector<GLPointData> pts;
    auto&& pt = shared_queue.front();
    GLPointData pt_conv{pt.x(), pt.y(), pt.z(), 1.0, 1.0, 1.0};
    pts.push_back(pt_conv);
    std::shared_ptr<entity::Points> new_pt(new entity::Points(pts));
    new_pt->setPointSize(5.0);
    renderer.addEntity(new_pt, "grpc_pt" + std::to_string(s_grpc_entity_count++));
    shared_queue.pop_front();
  }
}

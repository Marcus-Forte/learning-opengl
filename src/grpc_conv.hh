#pragma once
#include "entities/points.hh"
#include "grpc_listener.hh"
#include "renderer.hpp"
/* GRPC conversion utilities. */
using namespace grpc_listener;

static GLPointData fromgRPCPt(const Point3& pt) {
  float x = pt.x();
  float y = pt.y();
  float z = pt.z();
  float r = pt.has_r() ? pt.r() : 1.0;
  float g = pt.has_g() ? pt.g() : 1.0;
  float b = pt.has_b() ? pt.b() : 1.0;
  return {x, y, z, r, g, b};
}

static std::shared_ptr<entity::Points> fromgRPC(const Point3& pt) {
  std::vector<GLPointData> pts;
  pts.push_back(fromgRPCPt(pt));
  std::shared_ptr<entity::Points> new_pt(new entity::Points(pts));
  new_pt->setPointSize(5.0);
  return new_pt;
}

static std::shared_ptr<entity::Points> fromgRPC(const PointCloud3& pointcloud) {
  std::vector<GLPointData> pts;
  for (const auto& point : pointcloud.points()) {
    pts.push_back(fromgRPCPt(point));
  }
  std::shared_ptr<entity::Points> new_pts(new entity::Points(pts));
  new_pts->setPointSize(5.0);
  return new_pts;
}

void processgRPCQueue(SharedQueue& shared_queue, Renderer& renderer) {
  static int s_grpc_entity_count = 0;
  if (shared_queue.point_queue.size()) {
    auto&& pt = shared_queue.point_queue.front();
    auto gl_pt = fromgRPC(pt);
    renderer.addEntity(gl_pt, "grpc_pt" + std::to_string(s_grpc_entity_count++));
    shared_queue.point_queue.pop_front();
  }

  if (shared_queue.pointcloud_queue.size()) {
    // Extract points
    auto&& pointcloud = shared_queue.pointcloud_queue.front();
    auto gl_pointcloud = fromgRPC(pointcloud);
    const std::string entity_name =
        pointcloud.has_entity_name() ? pointcloud.entity_name() : "grpc_pcloud" + std::to_string(s_grpc_entity_count);
    renderer.addEntity(gl_pointcloud, entity_name);
    shared_queue.pointcloud_queue.pop_front();
  }
}

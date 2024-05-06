#pragma once
#include "entity/factory.hh"
#include "grpc_listener.hh"
#include "layouts/pointAttribute.hpp"
#include "renderer.hh"

// TODO mutex use?

/* GRPC conversion utilities. */
using namespace grpc_listener;

static GLPointData fromgRPCPt(const Point3 &pt) {
  float x = pt.x();
  float y = pt.y();
  float z = pt.z();
  float r = pt.has_r() ? pt.r() : 1.0;
  float g = pt.has_g() ? pt.g() : 1.0;
  float b = pt.has_b() ? pt.b() : 1.0;
  return {x, y, z, r, g, b};
}

static std::shared_ptr<entity::Points> fromgRPC(const Point3 &pt, const std::shared_ptr<entity::EntityFactory> &factory,
                                                float point_size = 5.0) {
  std::vector<GLPointData> pts;
  pts.push_back(fromgRPCPt(pt));
  auto new_pt = factory->create_points(pts);
  new_pt->setPointSize(point_size);
  return new_pt;
}

static std::shared_ptr<entity::Points> fromgRPC(const NamedPoint3 &pt,
                                                const std::shared_ptr<entity::EntityFactory> &factory) {
  std::vector<GLPointData> pts;
  pts.push_back(fromgRPCPt(pt.point()));
  auto new_pt = factory->create_points(pts);
  new_pt->setPointSize(pt.size());
  return new_pt;
}

static std::shared_ptr<entity::Points> fromgRPC(const PointCloud3 &pointcloud,
                                                const std::shared_ptr<entity::EntityFactory> &factory) {
  std::vector<GLPointData> pts;
  for (const auto &point : pointcloud.points()) {
    pts.push_back(fromgRPCPt(point));
  }
  auto new_pts = factory->create_points(pts);
  auto point_size = pointcloud.has_point_size() ? pointcloud.point_size() : 10.0;

  new_pts->setPointSize(point_size);
  return new_pts;
}

inline void processgRPCQueue(SharedQueue &shared_queue, Renderer &renderer,
                             const std::shared_ptr<entity::EntityFactory> &factory) {
  static int s_grpc_entity_count = 0;

  while (!shared_queue.point_queue.empty()) {
    auto &&pt = shared_queue.point_queue.front();
    auto gl_pt = fromgRPC(pt, factory);
    renderer.addEntity(gl_pt, "grpc_pt" + std::to_string(s_grpc_entity_count++));

    shared_queue.mutex.lock();
    shared_queue.point_queue.pop_front();
    shared_queue.mutex.unlock();
  }

  while (!shared_queue.pointcloud_queue.empty()) {
    // Extract points
    auto pointcloud = shared_queue.pointcloud_queue.front();
    auto &&gl_pointcloud = fromgRPC(pointcloud, factory);
    const std::string entity_name =
        pointcloud.has_entity_name() ? pointcloud.entity_name() : "grpc_pcloud" + std::to_string(s_grpc_entity_count++);
    renderer.addEntity(gl_pointcloud, entity_name);

    shared_queue.mutex.lock();
    shared_queue.pointcloud_queue.pop_front();
    shared_queue.mutex.unlock();
  }

  while (!shared_queue.named_point_queue.empty()) {
    auto named_point = shared_queue.named_point_queue.front();
    auto &&gl_Point = fromgRPC(named_point, factory);
    const std::string entity_name =
        named_point.has_name() ? named_point.name() : "grpc_named_pt" + std::to_string(s_grpc_entity_count++);
    renderer.addEntity(gl_Point, entity_name);

    shared_queue.mutex.lock();
    shared_queue.named_point_queue.pop_front();
    shared_queue.mutex.unlock();
  }
}

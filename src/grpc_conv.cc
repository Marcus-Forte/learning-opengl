#include "grpc_conv.hh"

/* @brief  gRPC <-> Renderer conversion processor.
 *
 */

using namespace grpc_listener;

static unsigned long g_grpc_entity_count = 0;

static GLPointData fromgRPCPt(const gl::Point3 &pt) {
  float x = pt.x();
  float y = pt.y();
  float z = pt.z();
  float r = pt.has_r() ? pt.r() : 1.0;
  float g = pt.has_g() ? pt.g() : 1.0;
  float b = pt.has_b() ? pt.b() : 1.0;
  return {x, y, z, r, g, b};
}

static std::shared_ptr<entity::Points> fromgRPC(const gl::Point3 &pt,
                                                const std::shared_ptr<entity::EntityFactory> &factory,
                                                float point_size = 5.0) {
  std::vector<GLPointData> pts;
  pts.push_back(fromgRPCPt(pt));
  auto new_pt = factory->create_points(pts);
  new_pt->setPointSize(point_size);
  return new_pt;
}

static std::shared_ptr<entity::Points> fromgRPC(const gl::PointCloud3 &pointcloud,
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

void processgRPCQueue(SharedQueue &shared_queue, Renderer &renderer,
                      const std::shared_ptr<entity::EntityFactory> &factory) {
  while (!shared_queue.point_queue.empty()) {
    auto &&pt = shared_queue.point_queue.front();
    auto gl_pt = fromgRPC(pt, factory);
    renderer.addEntity(gl_pt, "grpc_pt" + std::to_string(g_grpc_entity_count++));

    shared_queue.point_queue.pop();
  }

  while (!shared_queue.pointcloud_queue.empty()) {
    auto pointcloud = shared_queue.pointcloud_queue.front();
    auto &&gl_pointcloud = fromgRPC(pointcloud, factory);
    const std::string entity_name =
        pointcloud.has_entity_name() ? pointcloud.entity_name() : "grpc_pcloud" + std::to_string(g_grpc_entity_count++);
    renderer.addEntity(gl_pointcloud, entity_name);

    shared_queue.pointcloud_queue.pop();
  }

  shared_queue.line_queue.consume_all([&](const gl::LinesRequest &lines) {
    for (const auto &line : lines.lines()) {
      const auto gl_line = factory->create_line(line.x0(), line.y0(), line.z0(), line.x1(), line.y1(), line.z1(),
                                                lines.r(), lines.g(), lines.b());
      const std::string entity_name =
          line.has_entity_name() ? line.entity_name() : "grpc_line" + std::to_string(g_grpc_entity_count++);

      renderer.addEntity(gl_line, entity_name);
    }
  });

  /// \todo be smarter adding axes
  if (shared_queue.reset_scene) {
    renderer.clearEntities();
    auto axis = factory->create_axis(0, 0, 0, 0.5);
    renderer.addEntity(axis, "axis");
    shared_queue.reset_scene = false;
    g_grpc_entity_count = 0;
  }
}

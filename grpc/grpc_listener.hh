#pragma once

#include <boost/lockfree/spsc_queue.hpp>

#include "grpcpp/grpcpp.h"
#include "proto/gl_server.grpc.pb.h"
#include "proto/gl_server.pb.h"

constexpr size_t g_num_elements = 1024;

/**
 * @brief lockfree queues. `grpc_listener.cc` is the producer (gRPC thread).
 */
using PointQueue = boost::lockfree::spsc_queue<gl::Point3, boost::lockfree::capacity<g_num_elements>>;
using PointCloudQueue = boost::lockfree::spsc_queue<gl::PointCloud3, boost::lockfree::capacity<g_num_elements>>;
using LineQueue = boost::lockfree::spsc_queue<gl::LinesRequest, boost::lockfree::capacity<g_num_elements>>;

namespace grpc_listener {
struct SharedQueue {
  PointQueue point_queue;
  PointCloudQueue pointcloud_queue;
  LineQueue line_queue;
  bool reset_scene;
};

class addToSceneImpl : public gl::addToScene::Service {
 public:
  addToSceneImpl(SharedQueue &queue) : queue_(queue) {}
  ::grpc::Status addPoint(::grpc::ServerContext *context, const gl::Point3 *request,
                          ::google::protobuf::Empty *response) override;
  ::grpc::Status addPointCloud(::grpc::ServerContext *context, const gl::PointCloud3 *request,
                               ::google::protobuf::Empty *response) override;
  ::grpc::Status addLines(::grpc::ServerContext *context, const ::gl::LinesRequest *request,
                          ::google::protobuf::Empty *response) override;
  ::grpc::Status streamPointClouds(::grpc::ServerContext *context, ::grpc::ServerReader<gl::PointCloud3> *reader,
                                   ::google::protobuf::Empty *response) override;

  ::grpc::Status resetScene(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                            ::google::protobuf::Empty *response) override;

 private:
  SharedQueue &queue_;
};

class gRPCListener {
 public:
  gRPCListener(SharedQueue &shared_queue) : shared_queue_(shared_queue) {}
  void start();
  void shutDown();

 private:
  SharedQueue &shared_queue_;
  std::unique_ptr<grpc::Server> server_;
  std::shared_ptr<addToSceneImpl> service_;
};
}  // namespace grpc_listener
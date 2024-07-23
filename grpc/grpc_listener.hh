#pragma once

#include <queue>
#include <thread>

#include "grpcpp/grpcpp.h"
#include "proto/points.grpc.pb.h"
#include "proto/points.pb.h"

using PointQueue = std::deque<gl::Point3>;
using PointCloudQueue = std::deque<gl::PointCloud3>;
using NamedPointQueue = std::deque<gl::NamedPoint3>;

namespace grpc_listener {
struct SharedQueue {
  PointQueue point_queue;
  PointCloudQueue pointcloud_queue;
  NamedPointQueue named_point_queue;
  bool reset_scene;
  std::mutex mutex;
};

class addToSceneImpl : public gl::addToScene::Service {
 public:
  addToSceneImpl(SharedQueue &queue) : queue_(queue) {}
  ::grpc::Status addPoint(::grpc::ServerContext *context, const gl::Point3 *request,
                          ::google::protobuf::Empty *response) override;
  ::grpc::Status addPointCloud(::grpc::ServerContext *context, const gl::PointCloud3 *request,
                               ::google::protobuf::Empty *response) override;
  ::grpc::Status streamPointClouds(::grpc::ServerContext *context, ::grpc::ServerReader<gl::PointCloud3> *reader,
                                   ::google::protobuf::Empty *response) override;
  ::grpc::Status streamNamedPoints(::grpc::ServerContext *context, ::grpc::ServerReader<gl::NamedPoint3> *reader,
                                   ::google::protobuf::Empty *response) override;

  ::grpc::Status resetScene(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                            ::google::protobuf::Empty *response) override;

 private:
  SharedQueue &queue_;
};

class gRPCListener {
 public:
  gRPCListener(SharedQueue &shared_queue) : shared_queue_(shared_queue) {}
  void startAsync();
  void shutDown();

 private:
  SharedQueue &shared_queue_;
  std::thread listener_thread_;
  std::unique_ptr<grpc::Server> server_;
};
}  // namespace grpc_listener
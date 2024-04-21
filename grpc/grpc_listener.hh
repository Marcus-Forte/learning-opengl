#pragma once
#include <queue>
#include <thread>

#include "grpcpp/grpcpp.h"
#include "proto/points.grpc.pb.h"
#include "proto/points.pb.h"

using PointQueue = std::deque<Point3>;
using PointCloudQueue = std::deque<PointCloud3>;

namespace grpc_listener {
struct SharedQueue {
  PointQueue point_queue;
  PointCloudQueue pointcloud_queue;
  std::mutex mutex;
};

class addToSceneImpl : public addToScene::Service {
 public:
  addToSceneImpl(SharedQueue &queue) : queue_(queue) {}
  ::grpc::Status addPoint(::grpc::ServerContext *context, const ::Point3 *request, ::google::protobuf::Empty *response);
  ::grpc::Status addPointCloud(::grpc::ServerContext *context, const ::PointCloud3 *request,
                               ::google::protobuf::Empty *response);
  ::grpc::Status steamPointClouds(::grpc::ServerContext *context, ::grpc::ServerReader<::PointCloud3> *reader,
                                  ::google::protobuf::Empty *response);

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
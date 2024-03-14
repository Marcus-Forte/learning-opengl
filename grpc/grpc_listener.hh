#pragma once
#include <queue>
#include <thread>

#include "grpc/points.grpc.pb.h"
#include "grpc/points.pb.h"
#include "grpcpp/grpcpp.h"

using PointQueue = std::deque<Point3>;
using PointCloudQueue = std::deque<PointCloud3>;

namespace grpc_listener {
struct SharedQueue {
  PointQueue point_queue;
  PointCloudQueue pointcloud_queue;
};

class addToSceneImpl : public addToScene::Service {
 public:
  addToSceneImpl(SharedQueue& queue) : queue_(queue) {}
  ::grpc::Status addPoint(::grpc::ServerContext* context, const ::Point3* request, ::google::protobuf::Empty* response);
  ::grpc::Status addPointCloud(::grpc::ServerContext* context, const ::PointCloud3* request,
                               ::google::protobuf::Empty* response);

 private:
  SharedQueue& queue_;
};

class gRPCListener {
 public:
  gRPCListener(SharedQueue& shared_queue) : shared_queue_(shared_queue) {}
  void startAsync();
  void shutDown();

 private:
  SharedQueue& shared_queue_;
  std::thread listener_thread_;
  std::unique_ptr<grpc::Server> server_;
};
}  // namespace grpc_listener
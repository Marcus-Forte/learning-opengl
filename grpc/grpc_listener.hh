#pragma once
#include <queue>

#include "grpc/data.grpc.pb.h"
#include "grpc/data.pb.h"
#include "grpcpp/grpcpp.h"
#include <thread>
using SharedQueue = std::deque<Point3>;

class addToSceneImpl : public addToScene::Service {
 public:
  addToSceneImpl(SharedQueue& queue) : queue_(queue) {}
  ::grpc::Status addPoint(::grpc::ServerContext* context, const ::Point3* request, ::google::protobuf::Empty* response);

 private:
  SharedQueue& queue_;
};

class gRPCListener {
  public:
  gRPCListener(std::deque<Point3>& shared_queue) : shared_queue_(shared_queue) {}
  void startAsync();
  void shutDown();

 private:
  SharedQueue& shared_queue_;
  std::thread listener_thread_;
  std::unique_ptr<grpc::Server> server_;
};
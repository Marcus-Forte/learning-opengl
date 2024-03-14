#include "grpc_listener.hh"
#include <grpcpp/ext/proto_server_reflection_plugin.h>

namespace grpc_listener {
::grpc::Status addToSceneImpl::addPoint(::grpc::ServerContext* context, const ::Point3* request,
                                        ::google::protobuf::Empty* response) {
  queue_.point_queue.push_front(*request);
  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::addPointCloud(::grpc::ServerContext* context, const ::PointCloud3* request,
                                             ::google::protobuf::Empty* response) {
  queue_.pointcloud_queue.push_front(*request);
  return ::grpc::Status::OK;
}
void gRPCListener::startAsync() {
  listener_thread_ = std::thread([this]() {
    addToSceneImpl service(shared_queue_);

    grpc::ServerBuilder builder;
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    const std::string ip_port = "0.0.0.0:50051";
    builder.AddListeningPort(ip_port, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    server_ = builder.BuildAndStart();

    std::cout << "gRPC Server listening on: " << ip_port << std::endl;
    server_->Wait();
  });
}

void gRPCListener::shutDown() {
  std::cout << "gRPC shutdown...\n";
  server_->Shutdown();
  listener_thread_.join();
}
}
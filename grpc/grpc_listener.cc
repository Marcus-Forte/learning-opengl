#include "grpc_listener.hh"

#include <grpcpp/ext/proto_server_reflection_plugin.h>

// TODO mutex use?

namespace grpc_listener {
::grpc::Status addToSceneImpl::addPoint(::grpc::ServerContext *context, const ::Point3 *request,
                                        ::google::protobuf::Empty *response) {
  queue_.mutex.lock();
  queue_.point_queue.push_front(*request);
  queue_.mutex.unlock();
  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::addPointCloud(::grpc::ServerContext *context, const ::PointCloud3 *request,
                                             ::google::protobuf::Empty *response) {
  queue_.mutex.lock();
  queue_.pointcloud_queue.push_front(*request);
  queue_.mutex.unlock();
  return ::grpc::Status::OK;
}

grpc::Status addToSceneImpl::steamPointClouds(::grpc::ServerContext *context,
                                              ::grpc::ServerReader<::PointCloud3> *reader,
                                              ::google::protobuf::Empty *response) {
  PointCloud3 cloud;

  while (reader->Read(&cloud)) {
    queue_.mutex.lock();
    queue_.pointcloud_queue.push_front(cloud);
    queue_.mutex.unlock();
  };

  return ::grpc::Status::OK;
}

grpc::Status addToSceneImpl::streamNamedPoints(::grpc::ServerContext *context,
                                               ::grpc::ServerReader<::NamedPoint3> *reader,
                                               ::google::protobuf::Empty *response) {
  NamedPoint3 point;

  while (reader->Read(&point)) {
    queue_.mutex.lock();
    queue_.named_point_queue.push_front(point);
    queue_.mutex.unlock();
  };

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
}  // namespace grpc_listener
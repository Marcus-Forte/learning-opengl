#include "grpc_listener.hh"

#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <mutex>

// TODO mutex use?

namespace grpc_listener {
::grpc::Status addToSceneImpl::addPoint(::grpc::ServerContext *context, const gl::Point3 *request,
                                        ::google::protobuf::Empty *response) {
  std::lock_guard<std::mutex> lock(queue_.mutex);
  queue_.point_queue.push_front(*request);

  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::addPointCloud(::grpc::ServerContext *context, const gl::PointCloud3 *request,
                                             ::google::protobuf::Empty *response) {
  std::lock_guard<std::mutex> lock(queue_.mutex);
  queue_.pointcloud_queue.push_front(*request);
  return ::grpc::Status::OK;
}

grpc::Status addToSceneImpl::streamPointClouds(::grpc::ServerContext *context,
                                               ::grpc::ServerReader<gl::PointCloud3> *reader,
                                               ::google::protobuf::Empty *response) {
  gl::PointCloud3 cloud;

  while (reader->Read(&cloud)) {
    std::lock_guard<std::mutex> lock(queue_.mutex);
    queue_.pointcloud_queue.push_front(cloud);
  };

  return ::grpc::Status::OK;
}

grpc::Status addToSceneImpl::streamNamedPoints(::grpc::ServerContext *context,
                                               ::grpc::ServerReader<gl::NamedPoint3> *reader,
                                               ::google::protobuf::Empty *response) {
  gl::NamedPoint3 point;

  while (reader->Read(&point)) {
    std::lock_guard<std::mutex> lock(queue_.mutex);
    queue_.named_point_queue.push_front(point);
  };

  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::resetScene(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                                          ::google::protobuf::Empty *response) {
  queue_.reset_scene = true;
  return ::grpc::Status::OK;
}

void gRPCListener::startAsync() {
  listener_thread_ = std::thread([this]() {
    addToSceneImpl service(shared_queue_);

    grpc::ServerBuilder builder;
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    const std::string ip_port = "0.0.0.0:50052";
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
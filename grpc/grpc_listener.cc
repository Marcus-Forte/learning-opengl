#include "grpc_listener.hh"

#include <grpcpp/ext/proto_server_reflection_plugin.h>

namespace grpc_listener {
::grpc::Status addToSceneImpl::addPoint(::grpc::ServerContext *context, const gl::Point3 *request,
                                        ::google::protobuf::Empty *response) {
  queue_.point_queue.push(*request);

  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::addPointCloud(::grpc::ServerContext *context, const gl::PointCloud3 *request,
                                             ::google::protobuf::Empty *response) {
  queue_.pointcloud_queue.push(*request);
  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::addLines(::grpc::ServerContext *context, const ::gl::LinesRequest *request,
                                        ::google::protobuf::Empty *response) {
  queue_.line_queue.push(*request);
  return ::grpc::Status::OK;
}

grpc::Status addToSceneImpl::streamPointClouds(::grpc::ServerContext *context,
                                               ::grpc::ServerReader<gl::PointCloud3> *reader,
                                               ::google::protobuf::Empty *response) {
  gl::PointCloud3 cloud;

  while (reader->Read(&cloud)) {
    queue_.pointcloud_queue.push(cloud);
  };

  return ::grpc::Status::OK;
}

::grpc::Status addToSceneImpl::resetScene(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                                          ::google::protobuf::Empty *response) {
  queue_.reset_scene = true;
  return ::grpc::Status::OK;
}

void gRPCListener::start() {
  service_ = std::make_shared<addToSceneImpl>(shared_queue_);
  grpc::ServerBuilder builder;
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  const std::string ip_port = "0.0.0.0:50051";
  builder.AddListeningPort(ip_port, grpc::InsecureServerCredentials());
  builder.RegisterService(service_.get());

  server_ = builder.BuildAndStart();

  std::cout << "gRPC Server listening on: " << ip_port << std::endl;
}

void gRPCListener::shutDown() {
  std::cout << "gRPC shutdown...\n";
  server_->Shutdown();
  server_->Wait();
}
}  // namespace grpc_listener
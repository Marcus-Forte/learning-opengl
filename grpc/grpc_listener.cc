#include "grpc_listener.hh"

::grpc::Status addToSceneImpl::addPoint(::grpc::ServerContext* context, const ::Point3* request,
                                        ::google::protobuf::Empty* response) {
  queue_.push_front(*request);
  return ::grpc::Status::OK;
}
void gRPCListener::startAsync() {
  listener_thread_ = std::thread([this]() {
    addToSceneImpl service(shared_queue_);

    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    server_ = builder.BuildAndStart();

    std::cout << "gRPC Server listening....\n";
    server_->Wait();
  });
}

void gRPCListener::shutDown() {
  std::cout << "gRPC shutdown...\n";
  server_->Shutdown();
  listener_thread_.join();
}
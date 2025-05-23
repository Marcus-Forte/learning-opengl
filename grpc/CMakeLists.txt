find_package(Protobuf CONFIG)
find_package(gRPC CONFIG)

if(gRPC_FOUND)
    message(STATUS "gRPC detected.")

    add_library(protolib 
    proto/gl_server.proto
    )

    target_link_libraries(protolib gRPC::grpc++ gRPC::grpc++_reflection)
    target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
    protobuf_generate(TARGET protolib LANGUAGE cpp)
    protobuf_generate(TARGET protolib LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
    add_library(grpc_listener
    grpc_listener.cc
    )

    target_link_libraries(grpc_listener protolib)
    target_include_directories(grpc_listener PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

endif()
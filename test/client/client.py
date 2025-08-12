# Simple gRPC client that strams a pointcloud of single point to the local C++ server.
import grpc
import time
import math

from google.protobuf.empty_pb2 import Empty
from proto_gen import gl_server_pb2
from proto_gen import gl_server_pb2_grpc


def point_cloud_generator(radius=2.0, speed=1.0):
    """Generates a stream of PointCloud3 messages with a single point rotating around the origin."""
    angle = 0.0
    angle_step = 0.05 * speed  # radians

    while True:
        try:
            x = radius * math.cos(angle)
            y = radius * math.sin(angle)
            z = 0.0

            # Create a single point with color
            point = gl_server_pb2.Point3(x=x, y=y, z=z, r=1.0, g=0.5, b=0.2)

            # Create a point cloud containing that single point.
            # Using a consistent entity_name allows the server to update the same entity.
            point_cloud = gl_server_pb2.PointCloud3(entity_name="rotating_point", point_size=20.0)
            point_cloud.points.append(point)

            print(f"Sending point at: ({x:.2f}, {y:.2f}, {z:.2f})")
            yield point_cloud

            angle += angle_step
            time.sleep(0.016)  # ~60 FPS
        except grpc.RpcError as e:
            print(f"gRPC stream closed or error occurred: {e.details()}")
            break
        except Exception as e:
            print(f"An error occurred in the generator: {e}")
            break


def run():
    """Connects to the gRPC server and starts streaming point clouds."""
    server_address = 'localhost:50051'
    print(f"Connecting to gRPC server at {server_address}...")
    try:
        with grpc.insecure_channel(server_address) as channel:
            stub = gl_server_pb2_grpc.addToSceneStub(channel)

            # Before starting, reset the scene to clear previous drawings
            print("Resetting the scene...")
            stub.resetScene(Empty())
            time.sleep(0.5)

            print("Starting to stream point clouds...")
            stub.streamPointClouds(point_cloud_generator())

    except grpc.RpcError as e:
        if e.code() == grpc.StatusCode.UNAVAILABLE:
            print(f"Error: Connection to the server at {server_address} failed. Is the server running?")

if __name__ == '__main__':
    run()


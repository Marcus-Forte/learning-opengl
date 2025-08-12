# Python Client Environment 

## Setup

`. ./setup.sh`

## Proto

`python -m grpc_tools.protoc -Iproto_gen=../../grpc/proto --python_out=. --pyi_out=. --grpc_python_out=. ../../grpc/proto/gl_server.proto`

## Run Streaming Client

`python client.py`

cmake_minimum_required (VERSION 3.4)

project(proto CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_STATE_MSG PROTO_STATE_MSG_HDRS pillar_state.proto)

set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
    CACHE INTERNAL "Path to generated protobuf files.")

add_library(proto SHARED ${PROTO_STATE_MSG} ${PROTO_STATE_MSG_HDRS})

target_link_libraries(proto ${PROTOBUF_LIBRARIES})
target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# set_target_properties(proto PROPERTIES POSITION_INDEPENDENT_CODE ON)

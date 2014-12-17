set(CPPSRCS server_main.cpp data_server.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS io.proto)
add_executable(dataserver ${CPPSRCS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(dataserver boost_system ${PROTOBUF_LIBRARIES})

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dataserver DESTINATION bin)

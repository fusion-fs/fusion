set(CPPSRCS server_main.cpp data_server.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/protocol)
add_executable(dataserver ${CPPSRCS})

target_link_libraries(dataserver io boost_system ${PROTOBUF_LIBRARIES})

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dataserver DESTINATION bin)

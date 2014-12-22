#set(CPPSRCS server_main.cpp data_server.cpp)
#include_directories(${PROJECT_SOURCE_DIR}/src/protocol)
include_directories(${PROJECT_SOURCE_DIR}/src/db)
set(CSRCS data_server.c)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(dataserver ${CPPSRCS} ${CSRCS})

#target_link_libraries(dataserver io boost_system ${PROTOBUF_LIBRARIES})
target_link_libraries(dataserver websockets)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dataserver DESTINATION bin)

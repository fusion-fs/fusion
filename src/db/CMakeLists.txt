set(CPPSRCS cppwrapper.cpp fusionfs.cpp)
set(CSRCS fusion.c)
include_directories(${PROJECT_SOURCE_DIR}/src/protocol)
add_executable(fusionfs ${CSRCS} ${CPPSRCS})
target_link_libraries(fusionfs fuse pthread rt redis3m)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fusionfs DESTINATION bin)

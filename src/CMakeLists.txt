set(CPPSRCS cppwrapper.cpp fusionfs.cpp)
set(CSRCS fusion.c)

add_executable(fusionfs ${CSRCS} ${CPPSRCS})
target_link_libraries(fusionfs fuse pthread rt)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fusionfs DESTINATION bin)

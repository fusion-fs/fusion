set(CPPSRCS cppwrapper.cpp fusionfs.cpp)
set(CSRCS fusion.c)
include_directories(${PROJECT_SOURCE_DIR}/src/protocol)
add_executable(fusionfs ${CSRCS} ${CPPSRCS})
target_link_libraries(fusionfs fuse pthread rt redis3m)

add_custom_target(
   hiredis
   COMMAND make install
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/hiredis
)
target_link_libraries(fusionfs  ${PROJECT_SOURCE_DIR}/hiredis/libhiredis.so)
add_dependencies(fusionfs hiredis)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fusionfs DESTINATION bin)

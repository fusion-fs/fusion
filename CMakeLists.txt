project(fusionfs-fuse)

cmake_minimum_required(VERSION 2.8)
INCLUDE (CheckIncludeFiles)

find_package(Protobuf REQUIRED)

set(FUSIONFS_VERSION_MAJOR 0)
set(FUSIONFS_VERSION_MINOR 1)
set(FUSIONFS_VERSION_PATCH 0)
set(FUSIONFS_VERSION_STRING ${FUSIONFS_VERSION_MAJOR}.${FUSIONFS_VERSION_MINOR}.${FUSIONFS_VERSION_PATCH})
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -pipe")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -pipe -ggdb")

# Select flags.
SET(CMAKE_C_FLAGS "-pipe -W -Wall -DFORTIFY_SOURCE=2")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_C_FLAGS_RELEASE "-O2")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -DDEBUG")

add_definitions(-D_FILE_OFFSET_BITS=64)

set(Boost_NO_BOOST_CMAKE ON)
#add_subdirectory(redis3m)
add_subdirectory(src)

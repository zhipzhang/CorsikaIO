cmake_minimum_required(VERSION 3.11)
project(CorsikaIO)

find_library(ZLIB_LIBRARY z PATHS /usr/lib /lib/x86_64-linux-gnu)
find_library(ZSTD_LIBRARY zstd PATHS /usr/lib /lib/x86_64-linux-gnu)
set(CORSIKAIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/googletest)
add_subdirectory(test)



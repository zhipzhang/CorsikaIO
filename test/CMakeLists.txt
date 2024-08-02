add_executable(runTests test_decompress.cpp ${CMAKE_SOURCE_DIR}/src/GzipDecompressor.cpp)

target_link_libraries(runTests gtest gtest_main ${ZLIB_LIBRARY} ${ZSTD_LIBRARY})
target_include_directories(runTests PRIVATE  ${CORSIKAIO_INCLUDE_DIR})

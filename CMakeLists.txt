cmake_minimum_required(VERSION 3.10.2)
project(cpp-pilot-hse)

set(CMAKE_CXX_STANDARD 17)
add_executable(archiver main.cpp)
add_executable("archiver_test" tests/test.cpp)
add_library(reader STATIC reader.cpp reader.h)
add_library(writer STATIC writer.cpp writer.h)
add_library(compress STATIC compress.cpp compress.h)
add_library(decompress STATIC decompress.cpp decompress.h)
add_library(heap STATIC heap.cpp heap.h)
add_library(trie STATIC trie.cpp)
target_link_libraries(archiver reader writer compress decompress heap trie)

#Setup google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/tests/mock DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
target_link_libraries("archiver_test" ${GTEST_LIBRARIES} pthread reader writer compress decompress heap trie)

enable_testing()
add_test("archiver_test" "./archiver_test")
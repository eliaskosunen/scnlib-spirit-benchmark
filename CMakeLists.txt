cmake_minimum_required(VERSION 3.2.3)
project(scnlib-spirit-benchmark CXX)

find_package(Boost REQUIRED)
find_package(scn CONFIG REQUIRED)

set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE INTERNAL "")
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE INTERNAL "")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE INTERNAL "")
add_subdirectory(google-benchmark EXCLUDE_FROM_ALL)

add_executable(scnlib-spirit-benchmark bench.cpp)
target_link_libraries(scnlib-spirit-benchmark PUBLIC scn::scn Boost::boost benchmark)
target_compile_features(scnlib-spirit-benchmark PUBLIC cxx_std_14)

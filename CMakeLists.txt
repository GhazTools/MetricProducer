cmake_minimum_required(VERSION 3.23)
project(MetricProducer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("lib")
include_directories("include")

find_package(PkgConfig REQUIRED)
pkg_check_modules(rdkafka REQUIRED IMPORTED_TARGET rdkafka)
pkg_check_modules(rdkafka++ REQUIRED IMPORTED_TARGET rdkafka++)

add_library(MetricProducer src/metric_producer.cpp)

target_link_libraries(MetricProducer PUBLIC PkgConfig::rdkafka)
target_link_libraries(MetricProducer PUBLIC PkgConfig::rdkafka++)
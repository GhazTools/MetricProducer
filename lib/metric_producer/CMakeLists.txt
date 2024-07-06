cmake_minimum_required(VERSION 3.23)
project(MetricProducer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the librdkafka package
find_package(PkgConfig REQUIRED)
pkg_check_modules(rdkafka REQUIRED IMPORTED_TARGET rdkafka)
add_executable(MetricProducer  main.cpp)

target_link_libraries(MetricProducer PUBLIC PkgConfig::rdkafka)

pkg_check_modules(rdkafka++ REQUIRED IMPORTED_TARGET rdkafka++)
target_link_libraries(MetricProducer PUBLIC PkgConfig::rdkafka++)
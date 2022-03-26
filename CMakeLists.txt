cmake_minimum_required(VERSION 3.14)

project(
  asio
  VERSION 1.22.1
  LANGUAGES CXX
)

include(cmake/CPM.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/asio/include)

add_library(asio INTERFACE)

target_include_directories(asio SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/asio/include)

target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)

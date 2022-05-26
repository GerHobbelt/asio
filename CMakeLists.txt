cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(asio)
include(GNUInstallDirs)
find_package(Threads REQUIRED)

add_library(asio_headeronly INTERFACE)
target_compile_features(asio_headeronly INTERFACE cxx_std_11)
target_compile_definitions(asio_headeronly INTERFACE
  ASIO_STANDALONE=1
  $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0601>  # Win7
)
target_include_directories(asio_headeronly INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/include>"
)
target_link_libraries(asio_headeronly INTERFACE Threads::Threads)

set(asio_source_file "${CMAKE_CURRENT_BINARY_DIR}/src.cpp")
file(WRITE "${asio_source_file}" "#include <asio/impl/src.hpp>")
add_library(asio_shared SHARED "${asio_source_file}")
target_compile_features(asio_shared PUBLIC cxx_std_11)
target_compile_definitions(asio_shared PUBLIC
  ASIO_STANDALONE=1
  ASIO_SEPARATE_COMPILATION=1
  ASIO_DYN_LINK=1
  $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0601>  # Win7
)
target_include_directories(asio_shared PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/include>"
)
target_link_libraries(asio_shared PUBLIC Threads::Threads)
set_target_properties(asio_shared PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN on)

add_library(asio_static STATIC "${asio_source_file}")
target_compile_features(asio_static PUBLIC cxx_std_11)
target_compile_definitions(asio_static PUBLIC
  ASIO_STANDALONE=1
  ASIO_SEPARATE_COMPILATION=1
  $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0601>  # Win7
)
target_include_directories(asio_static PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/include>"
)
target_link_libraries(asio_static PUBLIC Threads::Threads)


install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/asio/include/asio" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/asio/include/asio.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS asio_headeronly asio_shared asio_static
  EXPORT asioExports
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
configure_file("config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/asioConfig.cmake" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/asioConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/asio"
)
install(EXPORT asioExports
  NAMESPACE asio::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/asio"
)

################################################################################
#    Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
#                                                                              #
#              This software is distributed under the terms of the             #
#              GNU Lesser General Public Licence (LGPL) version 3,             #
#                  copied verbatim in the file "LICENSE"                       #
################################################################################

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
cmake_policy(VERSION 3.14...3.20)

project(asio VERSION 1.19.1 LANGUAGES CXX)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "${PROJECT_NAME} v${PROJECT_VERSION} - ${CMAKE_BUILD_TYPE}")

include(GNUInstallDirs)

set(PROJECT_EXPORT_SET ${PROJECT_NAME}-targets)

add_library(asio INTERFACE)
add_library(${PROJECT_NAME}::asio ALIAS asio)
target_include_directories(asio INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

include(CMakePackageConfigHelpers)

set(PACKAGE_INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)
configure_package_config_file(
  "${PROJECT_NAME}-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION ${PACKAGE_INSTALL_DESTINATION}
  PATH_VARS CMAKE_INSTALL_PREFIX
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
  ARCH_INDEPENDENT
)

install(DIRECTORY asio/include/asio
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.*pp"
)
install(FILES asio/include/asio.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(TARGETS asio
  EXPORT ${PROJECT_EXPORT_SET}
)
install(EXPORT ${PROJECT_EXPORT_SET}
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${PACKAGE_INSTALL_DESTINATION}
  EXPORT_LINK_INTERFACE_LIBRARIES
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION ${PACKAGE_INSTALL_DESTINATION}
)
install(FILES asio/COPYING asio/LICENSE_1_0.txt
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
)

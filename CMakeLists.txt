cmake_minimum_required(VERSION 3.14)

project(
  asio
  VERSION 1.22.1
  LANGUAGES CXX
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/asio/include)

add_library(${PROJECT_NAME} INTERFACE)

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/asio/include/*.h")

target_sources(${PROJECT_NAME} INTERFACE ${HEADERS})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_BINARY_DIR}/asio/include)

target_compile_definitions(${PROJECT_NAME} INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)

# 1) If _WIN32_WINNT is not set, ASIO assumes _WIN32_WINNT=0x0501, i.e. Windows XP target, which is
# definitely not the platform which most users target.
#
# 2) WIN32_LEAN_AND_MEAN is defined to make Winsock2 work.
if (WIN32)
  # macro see @ https://stackoverflow.com/a/40217291/1746503
  macro(get_win32_winnt version)
    if (CMAKE_SYSTEM_VERSION)
      set(ver ${CMAKE_SYSTEM_VERSION})
      string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
      string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
      # Check for Windows 10, b/c we'll need to convert to hex 'A'.
      if ("${verMajor}" MATCHES "10")
        set(verMajor "A")
        string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
      endif ("${verMajor}" MATCHES "10")
      # Remove all remaining '.' characters.
      string(REPLACE "." "" ver ${ver})
      # Prepend each digit with a zero.
      string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
      set(${version} "0x${ver}")
    endif ()
  endmacro()

  if (NOT DEFINED _WIN32_WINNT)
    get_win32_winnt(ver)
    set(_WIN32_WINNT ${ver})
  endif ()

  message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

  target_compile_definitions(${PROJECT_NAME} INTERFACE _WIN32_WINNT=${_WIN32_WINNT} WIN32_LEAN_AND_MEAN)
endif ()

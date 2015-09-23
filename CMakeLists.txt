cmake_minimum_required (VERSION 2.8)
project(NetZ)

option(BUILD_TESTS "Build unit tests" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  add_definitions(-std=c++11
                  -Wall
                  -Werror
                  -g
                  -fPIC)
elseif (MSVC)
  if(NOT CMAKE_GENERATOR STREQUAL "Visual Studio 14 2015")
    message(FATAL_ERROR "This project requires Visual Studio 2015. Exiting.")  
  endif ()
endif ()

if (CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
      "Creating default configuration types" FORCE)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

if (CMAKE_SYSTEM_NAME MATCHES ".*Linux")
  set(THREAD_LIB pthread)
  set(TEST_LIB ${CMAKE_SOURCE_DIR}/external/gmock/lib/libgmock.a)
  set(TEST_LIB_DEBUG ${CMAKE_SOURCE_DIR}/external/gmock/lib/libgmock.a)
  add_definitions(-DPLATFORM_LINUX)
elseif (WIN32)
  add_definitions(-DPLATFORM_WINDOWS -D_CRT_SECURE_NO_WARNINGS)
  set(TEST_LIB ${CMAKE_SOURCE_DIR}/external/gmock/lib/release/gmock.lib)
  set(TEST_LIB_DEBUG ${CMAKE_SOURCE_DIR}/external/gmock/lib/debug/gmock.lib)
else ()
  message(FATAL_ERROR "Unsupported platform! Exiting.")
endif ()

if (MSVC)
  add_definitions(/wd4068 /wd4669)
endif ()

add_subdirectory(libraries/network)
add_subdirectory(projects)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif ()

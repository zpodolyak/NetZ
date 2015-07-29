cmake_minimum_required (VERSION 2.8)
project(netz)

option(BUILD_TESTS "Build unit tests" OFF)

include_directories(include)

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

file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

if (CMAKE_SYSTEM_NAME MATCHES ".*Linux")
  set(PLATFORM_DIR src/linux/)
  set(THREAD_LIB pthread)
  set(TEST_LIB ${CMAKE_SOURCE_DIR}/external/gmock/lib/libgmock.a)
  add_definitions(-DPLATFORM_LINUX)
elseif (WIN32)
  set(PLATFORM_DIR src/windows/)
  set(TEST_LIB ${CMAKE_SOURCE_DIR}/external/gmock/lib/gmock.lib)
  add_definitions(-DPLATFORM_WINDOWS -D_CRT_SECURE_NO_WARNINGS)
  file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/*h")
  source_group("include" FILES ${HEADERS})
  source_group("src" FILES ${LIB_SOURCES})
else ()
  message(FATAL_ERROR "Unsupported platform! Exiting.")
endif ()

file(GLOB PLATFORM_LIB_SOURCES "${PLATFORM_DIR}/*.cpp")

add_library(netz STATIC ${HEADERS} ${LIB_SOURCES} ${PLATFORM_LIB_SOURCES})

if (MSVC)
  source_group("src\\windows" FILES ${PLATFORM_LIB_SOURCES})
  add_definitions(/wd4068 /wd4669)
endif ()

add_executable(test_app src/test_app.cpp)
target_link_libraries(test_app netz)

if (BUILD_TESTS)
  add_subdirectory(test)
endif ()

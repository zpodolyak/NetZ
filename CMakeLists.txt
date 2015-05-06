cmake_minimum_required (VERSION 2.8)
project(netz)

option(BUILD_TESTS "Build unit tests" OFF)

include_directories(include)

add_definitions(-std=c++11
                -Wall
                -Werror
                -g
                -fPIC)

file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
  set(PLATFORM_DIR src/linux/)
  set(THREAD_LIB pthread)
  set(TEST_LIB ${CMAKE_SOURCE_DIR}/external/gmock/lib/libgmock.a)
  list(APPEND LIB_SOURCES ${PLATFORM_DIR}/epoll_reactor.cpp)
  add_definitions(-DPLATFORM_LINUX)
else()
  message(FATAL_ERROR "Unsupported platform! Exiting.")
endif()

add_library(netz STATIC ${LIB_SOURCES})

add_executable(test_app src/test_app.cpp)
target_link_libraries(test_app netz)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

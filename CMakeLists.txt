cmake_minimum_required (VERSION 2.8)
project(netz)

include_directories(include)

add_definitions(-std=c++11
                -Wall
                -Werror
                -g
                -fPIC)

if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
      set(PLATFORM_DIR src/platform/linux/)
      set(THREAD_LIB pthread)
      add_definitions(-DPLATFORM_LINUX)
else()
  message(FATAL_ERROR "Unsupported platform! Exiting.")
endif()

set(LIB_SOURCES
    ${PLATFORM_DIR}/reactor.cpp
    src/address.cpp
    src/socket.cpp)

add_library(netz STATIC ${LIB_SOURCES})

add_executable(test_app src/test_app.cpp)
target_link_libraries(test_app netz)
    
add_subdirectory(test)

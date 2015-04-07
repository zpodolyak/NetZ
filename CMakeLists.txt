cmake_minimum_required (VERSION 2.8)
project(netzwerk)

include_directories(include)

add_definitions(-std=c++11
                -Wall
                -Werror
                -g
                -fPIC)

if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
      set(PLATFORM_DIR src/platform/linux/)
      add_definitions(-DPLATFORM_LINUX)
endif()

set(SOURCES
    ${PLATFORM_DIR}/reactor.cpp
    src/s_socket.cpp
    src/socket_address.cpp
    src/socket_server.cpp
    src/test.cpp)

add_executable(test ${SOURCES})    


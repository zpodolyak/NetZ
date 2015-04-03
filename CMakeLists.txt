cmake_minimum_required (VERSION 2.8)
project(netzwerk)

include_directories(include)

add_definitions(-std=c++11
                -Wall
                -Werror
                -g
                -fPIC)

set(SOURCES
    common/epoll_reactor.cpp
    s_socket.cpp
    socket_address.cpp
    socket_server.cpp
    test.cpp)

add_executable(test ${SOURCES})    


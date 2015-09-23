include_directories(${CMAKE_SOURCE_DIR})

add_executable(example_tcp_server examples/example_tcp_server.cpp)
target_link_libraries(example_tcp_server network)

add_executable(example_tcp_client examples/example_tcp_client.cpp)
target_link_libraries(example_tcp_client network)


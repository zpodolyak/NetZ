include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/external
                    ${CMAKE_SOURCE_DIR}/libraries/network/include)

set(TEST_SOURCES
    network/test_socket.cpp
    network/test_timer.cpp
    http/test_http_parser.cpp
    main.cpp )
    
if (MSVC)
  source_group("src" FILES "main.cpp")
  source_group("src\\network" FILES "network/test_socket.cpp" "network/test_timer.cpp")
  source_group("src\\http" FILES "http/test_http_parser.cpp")
endif ()

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests ${THREAD_LIB} debug ${TEST_LIB_DEBUG} optimized ${TEST_LIB})


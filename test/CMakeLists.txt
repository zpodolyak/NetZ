include_directories(../external
                    ../include)

set(TEST_SOURCES
    main.cpp 
    test_socket.cpp)

add_executable(test ${TEST_SOURCES})
target_link_libraries(test ${THREAD_LIB} ${TEST_LIB})


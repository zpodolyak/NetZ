include_directories(../external
                    ../include)

set(TEST_SOURCES
    main.cpp 
    test_socket.cpp)

add_executable(Test ${TEST_SOURCES})
target_link_libraries(Test ${THREAD_LIB} debug ${TEST_LIB_DEBUG} optimized ${TEST_LIB})


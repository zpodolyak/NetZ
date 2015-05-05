include_directories(../external
                    ../include)
 
add_executable(test main.cpp)
target_link_libraries(test ${THREAD_LIB} ${TEST_LIB})


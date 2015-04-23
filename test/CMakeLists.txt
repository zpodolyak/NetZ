include_directories(../external
                    ../include)

add_executable(test main.cpp)
target_link_libraries(test netz ${THREAD_LIB} ${CMAKE_SOURCE_DIR}/external/gmock/lib/libgmock.a)


include_directories(${CMAKE_SOURCE_DIR})

file(GLOB COMMON_HEADERS "*.h")
file(GLOB COMMON_SOURCES "*.cpp")

if (MSVC)
  source_group("src" FILES ${COMMON_HEADERS} ${COMMON_SOURCES})
endif()

add_custom_target(common SOURCES ${COMMON_HEADERS})


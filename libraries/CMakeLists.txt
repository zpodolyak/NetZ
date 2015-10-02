include_directories(${CMAKE_SOURCE_DIR})

file(GLOB COMMON_HEADERS "*.h")

if (MSVC)
  source_group("src" FILES ${COMMON_HEADERS})
endif()

add_custom_target(common SOURCES ${COMMON_HEADERS})


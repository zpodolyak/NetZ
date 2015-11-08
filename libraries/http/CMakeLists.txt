include_directories(${CMAKE_SOURCE_DIR})

file(GLOB HEADERS "*.h")
file(GLOB LIB_SOURCES "*.cpp")

if (MSVC)
  source_group("src" FILES ${HEADERS} ${LIB_SOURCES})
endif()

add_library(http STATIC ${HEADERS} ${LIB_SOURCES})


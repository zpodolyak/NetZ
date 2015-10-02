include_directories(${CMAKE_SOURCE_DIR} include)

file(GLOB HEADERS "include/*.h")
file(GLOB LIB_SOURCES "src/*.cpp")
file(GLOB PLATFORM_LIB_SOURCES "include/${PLATFORM_DIR}/*.h" "src/${PLATFORM_DIR}/*.cpp")

if (MSVC)
  source_group("include" FILES ${HEADERS})
  source_group("src" FILES ${LIB_SOURCES})
  source_group("src\\windows" FILES ${PLATFORM_LIB_SOURCES})
endif()

add_library(network STATIC ${HEADERS} ${LIB_SOURCES} ${PLATFORM_LIB_SOURCES})


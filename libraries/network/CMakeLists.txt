include_directories(include)

if (CMAKE_SYSTEM_NAME MATCHES ".*Linux")
  set(PLATFORM_DIR linux)
elseif (WIN32)
  set(PLATFORM_DIR windows)
endif()

file(GLOB HEADERS "include/*.h")
file(GLOB LIB_SOURCES "src/*.cpp")
file(GLOB PLATFORM_LIB_SOURCES "include/${PLATFORM_DIR}/*.h" "src/${PLATFORM_DIR}/*.cpp")

if (MSVC)
  source_group("include" FILES ${HEADERS})
  source_group("src" FILES ${LIB_SOURCES})
  source_group("src\\windows" FILES ${PLATFORM_LIB_SOURCES})
endif()

add_library(network STATIC ${HEADERS} ${LIB_SOURCES} ${PLATFORM_LIB_SOURCES})


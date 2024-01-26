set(target hyped_debug)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(${target} STATIC ${headers} ${code})
include_directories(
  ${target} "${CMAKE_SOURCE_DIR}/lib" "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(
  ${target}
  hyped_core
  hyped_io
  hyped_sensors
  hyped_motors
  ${CURSES_LIBRARIES}
)

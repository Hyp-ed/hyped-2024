set(target hyped_navigation_control)
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(${target} STATIC ${headers} ${code})
include_directories(
  ${target}
  INTERFACE
  "${CMAKE_SOURCE_DIR}/lib"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/lib/navigation"
)
target_link_libraries(
  ${target}
  Eigen3::Eigen
  hyped_core
  tomlplusplus::tomlplusplus
  hyped_navigation_filters
  hyped_navigation_preprocessing
)

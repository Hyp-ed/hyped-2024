set(target hyped_motors)
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(${target} STATIC ${headers} ${code})
include_directories(${target}
  INTERFACE
  "${CMAKE_SOURCE_DIR}/lib"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  )
target_include_directories(${target} PUBLIC ${rapidjson_SOURCE_DIR}/include)
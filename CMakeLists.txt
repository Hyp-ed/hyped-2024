cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_PATH clang-tidy)
if(NOT CLANG_TIDY_PATH)
  message(FATAL_ERROR "Could not find the program clang-tidy")
endif()
set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_PATH}; --quiet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wpedantic ")
set(CMAKE_CXX_LINKER_FLAGS
    "${CMAKE_CXX_LINKER_FLAGS} -stdlib=libc++ -std=c++20"
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(project HYPED)
project(${project} CXX)
enable_testing()
find_package(Eigen3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
find_package(Boost REQUIRED)
find_package(PahoMqttCpp REQUIRED)
include_directories(${PahoMqttCpp_INCLUDE_DIRS})
include(FetchContent)
FetchContent_Declare(
  rapidjson GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG master
)
FetchContent_GetProperties(rapidjson)
if(NOT rapidjson_POPULATED)
  FetchContent_Populate(rapidjson)
endif()
FetchContent_Declare(
  tomlplusplus GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
if(NOT tomlplusplus_POPULATED)
  FetchContent_Populate(tomlplusplus)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)

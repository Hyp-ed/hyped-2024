# Portions of this code are derived from
# https://github.com/ObKo/stm32-cmake/tree/master?tab=MIT-1-ov-file#readme
# available under the MIT License. You may obtain a copy of the MIT License at
#
# https://opensource.org/licenses/MIT

cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_STM32 "Build STM32 targets" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wpedantic ")

if(BUILD_STM32)
  set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} --specs=nosys.specs")
else()
  set(CMAKE_CXX_LINKER_FLAGS
      "${CMAKE_CXX_LINKER_FLAGS} -stdlib=libc++ -std=c++20"
  )
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(project HYPED)

if(BUILD_STM32)
  set(TOOLCHAIN_INC_PATH "/usr/arm-none-eabi/include")
  set(TOOLCHAIN_LIB_PATH "$/usr/arm-none-eabi/lib")

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

  find_program(CMAKE_C_COMPILER NAMES arm-none-eabi-gcc)
  find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-g++)
  find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc)

  set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
  set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
  set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
endif()

project(${project} CXX)
if(NOT BUILD_STM32)
  enable_testing()
endif()

find_package(Eigen3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})
if(NOT BUILD_STM32)
  find_package(Curses REQUIRED)
  include_directories(${CURSES_INCLUDE_DIR})
endif()
find_package(Boost REQUIRED)
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

if(BUILD_STM32)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -specs=nosys.specs -T${CMAKE_SOURCE_DIR}/F401RE.ld"
  )
  # set(STM32_FETCH_FAMILIES F4 F7)

  # set(STM32_FETCH_CUBE_VERSIONS v1.26.1 v1.16.1)

  # set(STM32_FETCH_CMSIS_VERSIONS v2.6.6 v1.2.6)

  # set(STM32_FETCH_HAL_VERSIONS v1.7.12 v1.2.9)
  set(FAMILY F4)
  set(FAMILY_L f4)
  set(CUBE_VERSION v1.26.1)
  set(CMSIS_VERSION v2.6.6)
  set(HAL_VERSION v1.7.12)

  set(HAL_COMP_LIST
      STM32F4
      RCC
      GPIO
      CORTEX
  )

  include(FetchContent)

  FetchContent_Declare(
    STM32-CMSIS
    GIT_REPOSITORY https://github.com/STMicroelectronics/cmsis_core/
    GIT_TAG v5.6.0
    GIT_PROGRESS TRUE
  )

  FetchContent_Declare(
    STM32CubeF4
    GIT_REPOSITORY https://github.com/STMicroelectronics/STM32CubeF4/
    GIT_TAG v1.26.1
    GIT_PROGRESS TRUE
  )

  FetchContent_Declare(
    STM32-CMSIS-F4
    GIT_REPOSITORY https://github.com/STMicroelectronics/cmsis_device_f4/
    GIT_TAG v2.6.6
    GIT_PROGRESS TRUE
  )

  FetchContent_Declare(
    STM32-HAL-F4
    GIT_REPOSITORY https://github.com/STMicroelectronics/stm32f4xx_hal_driver/
    GIT_TAG v1.7.12
    GIT_PROGRESS TRUE
  )

  # FetchContent_Declare( STM32CubeF7 GIT_REPOSITORY
  # https://github.com/STMicroelectronics/STM32CubeF7/ GIT_TAG v1.16.1
  # GIT_PROGRESS TRUE )

  # FetchContent_Declare( STM32-CMSIS-F7 GIT_REPOSITORY
  # https://github.com/STMicroelectronics/cmsis_device_f7/ GIT_TAG v1.2.6
  # GIT_PROGRESS TRUE )

  # FetchContent_Declare( STM32-HAL-F7 GIT_REPOSITORY
  # https://github.com/STMicroelectronics/stm32f7xx_hal_driver/ GIT_TAG v1.2.9
  # GIT_PROGRESS TRUE )

  set(STM32_F4_TYPES F401xE)
  set(STM32_F4_TYPE_MATCH "F401.[ED]")
  set(STM32_F4_RAM_SIZES 96K)
  set(STM32_F4_CCRAM_SIZES 0K)

  add_library(STM32::F4 INTERFACE IMPORTED)

  target_compile_options(
    STM32::F4
    INTERFACE -mthumb
              -Wall
              -ffunction-sections
              -fdata-sections
  )

  target_link_options(
    STM32::F4
    INTERFACE
    -mthumb
    -Wl,--gc-sections
  )
  target_compile_definitions(STM32::F4 INTERFACE STM32F4)
  add_library(STM32::F401xE INTERFACE IMPORTED)
  target_link_libraries(STM32::F401xE INTERFACE STM32::F4)
  target_compile_definitions(STM32::F401xE INTERFACE STM32F401xE)

  target_compile_options(
    STM32::F4 INTERFACE -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
  )
  target_link_options(
    STM32::F4
    INTERFACE
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
  )

  # set(STM32_F7_TYPES F767xx) set(STM32_F7_TYPE_MATCH "F767..")
  # set(STM32_F7_RAM_SIZES 512K) set(STM32_F7_CCRAM_SIZES 0K)

  # stm32_util_create_family_targets(F7)

  # target_compile_options( STM32::F7 INTERFACE -mcpu=cortex-m7
  # -mfpu=fpv5-sp-d16 -mfloat-abi=hard ) target_link_options( STM32::F7
  # INTERFACE -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard )

  FetchContent_MakeAvailable(STM32-CMSIS)
  set(STM32_CMSIS_PATH ${stm32-cmsis_SOURCE_DIR})
  FetchContent_MakeAvailable(STM32-CMSIS-F4)
  set(STM32_CMSIS_F4_PATH ${stm32-cmsis-f4_SOURCE_DIR})
  FetchContent_MakeAvailable(STM32-HAL-F4)
  set(STM32_HAL_F4_PATH ${stm32-hal-f4_SOURCE_DIR})

  find_package(CMSIS COMPONENTS STM32F4 REQUIRED)
  find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)

  add_library(STM32::NoSys INTERFACE IMPORTED)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
if(NOT BUILD_STM32)
  add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()

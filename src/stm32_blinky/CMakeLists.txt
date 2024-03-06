set(target stm32-blinky-f4)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/stm32.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# get_filename_component(STM32_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
# list(APPEND CMAKE_MODULE_PATH ${STM32_CMAKE_DIR}/cmake)

# include(stm32/common) include(stm32/devices)

file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CMAKE_C_COMPILER NAMES arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-gcc-g++)
find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc-gcc)

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

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
# Set compiler flags for target -Wall: all warnings activated
# -ffunction-sections -fdata-sections: remove unused code
target_compile_options(
  STM32::F4
  INTERFACE -mthumb
            -Wall
            -ffunction-sections
            -fdata-sections
)
# Set linker flags -mthumb: Generate thumb code -Wl,--gc-sections: Remove unused
# code
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

# target_compile_options( STM32::F7 INTERFACE -mcpu=cortex-m7 -mfpu=fpv5-sp-d16
# -mfloat-abi=hard ) target_link_options( STM32::F7 INTERFACE -mcpu=cortex-m7
# -mfpu=fpv5-sp-d16 -mfloat-abi=hard )

FetchContent_MakeAvailable(STM32-CMSIS)
set(STM32_CMSIS_PATH ${stm32-cmsis_SOURCE_DIR})
FetchContent_MakeAvailable(STM32-CMSIS-F4)
set(STM32_CMSIS_F4_PATH ${stm32-cmsis-f4_SOURCE_DIR})

FetchContent_MakeAvailable(STM32-HAL-F4)
set(STM32_HAL_F4_PATH ${stm32-hal-f4_SOURCE_DIR})

find_package(CMSIS COMPONENTS STM32F4 REQUIRED)
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)

add_library(STM32::NoSys INTERFACE IMPORTED)
message(STATUS "STM32::NoSys")

add_executable(stm32-blinky-f4 ${code} stm32f4xx_hal_conf.h)
target_link_libraries(
  stm32-blinky-f4
  HAL::STM32::F4::RCC
  HAL::STM32::F4::GPIO
  HAL::STM32::F4::CORTEX
  CMSIS::STM32::F401RE
  STM32::NoSys
)

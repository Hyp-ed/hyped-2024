cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)
set(CMAKE_BUILD_TYPE Debug)

project(
  stm32-blinky
  C
  CXX
  ASM
)
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CMAKE_C_COMPILER NAMES arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-gcc-g++)
find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc-gcc)

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

set(STM32_FETCH_FAMILIES F4 F7)
set(STM32_FETCH_CUBE_VERSIONS v1.26.1 v1.16.1)
set(STM32_FETCH_CMSIS_VERSIONS v2.6.6 v1.2.6)
set(STM32_FETCH_HAL_VERSIONS v1.7.12 v1.2.9)

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

# include(FetchContent)

# FetchContent_Declare( STM32-CMSIS GIT_REPOSITORY #
# https://github.com/STMicroelectronics/cmsis_core/ GIT_TAG v5.6.0 GIT_PROGRESS
# TRUE ) FetchContent_MakeAvailable(STM32-CMSIS)

# FetchContent_Declare( STM32-CMSIS-${FAMILY} GIT_REPOSITORY
# https://github.com/STMicroelectronics/cmsis_device_${FAMILY_L}/ GIT_TAG
# ${CMSIS_VERSION} GIT_PROGRESS TRUE )
# FetchContent_MakeAvailable(STM32-CMSIS-${FAMILY})

# FetchContent_Declare( STM32-HAL-${FAMILY} GIT_REPOSITORY
# https://github.com/STMicroelectronics/stm32${FAMILY_L}xx_hal_driver/ GIT_TAG
# ${HAL_VERSION} GIT_PROGRESS TRUE )
# FetchContent_MakeAvailable(STM32-HAL-${FAMILY})
stm32_fetch_cmsis(F4)
stm32_fetch_hal(F4)

find_package(CMSIS COMPONENTS STM32F4 REQUIRED)
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)

add_executable(stm32-blinky-f4 ${code} stm32f4xx_hal_conf.h)
target_link_libraries(
  stm32-blinky-f4
  HAL::STM32::F4::RCC
  HAL::STM32::F4::GPIO
  HAL::STM32::F4::CORTEX
  CMSIS::STM32::F401RE
  STM32::NoSys
)
stm32_print_size_of_target(stm32-blinky-f4)

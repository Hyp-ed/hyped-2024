cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/stm32_gcc.cmake
)
set(CMAKE_BUILD_TYPE Debug)

project(
  stm32-blinky
  C
  CXX
  ASM
)
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(HAL_COMP_LIST
    STM32F4
    RCC
    GPIO
    CORTEX
)

stm32_fetch_cmsis(F4)
stm32_fetch_hal(F4)

find_package(CMSIS COMPONENTS STM32F4 REQUIRED)
find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)

# Find all device specific drivers: find_package(HAL COMPONENTS STM32L0 STM32F1
# STM32F4 REQUIRED) Find drivers for all families: find_package(HAL COMPONENTS
# RCC GPIO CORTEX REQUIRED) Find LL driver: find_package(HAL COMPONENTS LL_GPIO
# REQUIRED) Find everything: find_package(HAL REQUIRED)

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

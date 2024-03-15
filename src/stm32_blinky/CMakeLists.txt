set(target stm32-blinky-f4)

file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_executable(stm32-blinky-f4 ${code} stm32f4xx_hal_conf.h)
target_link_libraries(
  stm32-blinky-f4
  HAL::STM32::F4::RCC
  HAL::STM32::F4::GPIO
  HAL::STM32::F4::CORTEX
  CMSIS::STM32::F401RE
  STM32::NoSys
)
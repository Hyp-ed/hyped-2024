set(STM32_F7_TYPES F767xx)
set(STM32_F7_TYPE_MATCH "F767..")
set(STM32_F7_RAM_SIZES 512K)
set(STM32_F7_CCRAM_SIZES 0K)

stm32_util_create_family_targets(F7)

target_compile_options(
  STM32::F7 INTERFACE -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard
)
target_link_options(
  STM32::F7
  INTERFACE
  -mcpu=cortex-m7
  -mfpu=fpv5-sp-d16
  -mfloat-abi=hard
)

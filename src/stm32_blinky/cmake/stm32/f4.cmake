set(STM32_F4_TYPES F401xE)
set(STM32_F4_TYPE_MATCH "F401.[ED]")
set(STM32_F4_RAM_SIZES 96K)
set(STM32_F4_CCRAM_SIZES 0K)

stm32_util_create_family_targets(F4)

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

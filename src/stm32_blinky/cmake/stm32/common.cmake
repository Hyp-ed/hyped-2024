set(STM32_SUPPORTED_FAMILIES_LONG_NAME STM32F4 STM32F7)
set(STM32_SUPPORTED_FAMILIES_SHORT_NAME F4 F7)

set(STM32_TARGET_TRIPLET "arm-none-eabi")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_SYSROOT "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}")
set(TOOLCHAIN_BIN_PATH "${STM32_TOOLCHAIN_PATH}/bin")
set(TOOLCHAIN_INC_PATH
    "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}/include"
)
set(TOOLCHAIN_LIB_PATH "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}/lib")

set(CMAKE_SYSROOT ${TOOLCHAIN_SYSROOT})

find_program(
  CMAKE_OBJCOPY NAMES ${STM32_TARGET_TRIPLET}-objcopy
  HINTS ${TOOLCHAIN_BIN_PATH}
)
find_program(
  CMAKE_OBJDUMP NAMES ${STM32_TARGET_TRIPLET}-objdump
  HINTS ${TOOLCHAIN_BIN_PATH}
)
find_program(
  CMAKE_SIZE NAMES ${STM32_TARGET_TRIPLET}-size HINTS ${TOOLCHAIN_BIN_PATH}
)
find_program(
  CMAKE_DEBUGGER NAMES ${STM32_TARGET_TRIPLET}-gdb HINTS ${TOOLCHAIN_BIN_PATH}
)
find_program(
  CMAKE_CPPFILT NAMES ${STM32_TARGET_TRIPLET}-c++filt
  HINTS ${TOOLCHAIN_BIN_PATH}
)

# This function adds a target with name '${TARGET}_always_display_size'. The new
# target builds a TARGET and then calls the program defined in CMAKE_SIZE to
# display the size of the final ELF.
function(stm32_print_size_of_target TARGET)
  add_custom_target(
    ${TARGET}_always_display_size ALL
    COMMAND ${CMAKE_SIZE} "$<TARGET_FILE:${TARGET}>"
    COMMENT "Target Sizes: "
    DEPENDS ${TARGET}
  )
endfunction()

add_library(STM32::NoSys INTERFACE IMPORTED)
target_compile_options(
  STM32::NoSys INTERFACE $<$<C_COMPILER_ID:GNU>:--specs=nosys.specs>
)
target_link_options(
  STM32::NoSys INTERFACE $<$<C_COMPILER_ID:GNU>:--specs=nosys.specs>
)

include(stm32/utilities)

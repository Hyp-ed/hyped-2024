if(${CMAKE_VERSION} VERSION_LESS "3.16.0")
  message(
    WARNING
      "Current CMake version is ${CMAKE_VERSION}. stm32-cmake requires CMake 3.16 or greater"
  )
endif()

get_filename_component(STM32_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
list(APPEND CMAKE_MODULE_PATH ${STM32_CMAKE_DIR})
message (STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

set(STM32_SUPPORTED_FAMILIES_LONG_NAME STM32F4)
set(STM32_SUPPORTED_FAMILIES_SHORT_NAME F4)

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

target_compile_options(
  STM32::NoSys INTERFACE $<$<C_COMPILER_ID:GNU>:--specs=nosys.specs>
)
target_link_options(
  STM32::NoSys INTERFACE $<$<C_COMPILER_ID:GNU>:--specs=nosys.specs>
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CMAKE_C_COMPILER NAMES arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-gcc-g++)
find_program(CMAKE_ASM_COMPILER NAMES arm-none-eabi-gcc-gcc)

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

# This should be safe to set for a bare-metal cross-compiler
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For information about why and how of this file:
# https://cmake.org/cmake/help/latest/command/find_package.html

set(CMSIS_FIND_COMPONENTS ${STM32_SUPPORTED_FAMILIES_LONG_NAME})

set(CMSIS_FIND_COMPONENTS_FAMILIES ${STM32_SUPPORTED_FAMILIES_LONG_NAME})
message(STATUS "Search for CMSIS families: ${CMSIS_FIND_COMPONENTS_FAMILIES}")

set(STM32_ALL_DEVICES F401RE)

# search for Include/cmsis_gcc.h
find_path(
  CMSIS_F4_CORE_PATH
  NAMES Include/cmsis_gcc.h
  PATHS "${STM32_CMSIS_PATH}" "${STM32_CUBE_F4_PATH}/Drivers/CMSIS"
  NO_DEFAULT_PATH
)

# search for Include/stm32[XX]xx.h
find_path(
  CMSIS_F4_PATH
  NAMES Include/stm32f4xx.h
  PATHS "${STM32_CMSIS_F4_PATH}"
        "${STM32_CUBE_F4_PATH}/Drivers/CMSIS/Device/ST/STM32F4xx"
  NO_DEFAULT_PATH
)

list(
  APPEND
  CMSIS_INCLUDE_DIRS
  "${CMSIS_F4_CORE_PATH}/Include"
  "${CMSIS_F4_PATH}/Include"
)

add_library(CMSIS::STM32::F4 INTERFACE IMPORTED)
# STM32::F4 contains compile options and is define in <family>.cmake
target_link_libraries(CMSIS::STM32::F4 INTERFACE STM32::F4)
target_include_directories(
  CMSIS::STM32::F4 INTERFACE "${CMSIS_F4_CORE_PATH}/Include"
)
target_include_directories(
  CMSIS::STM32::F4 INTERFACE "${CMSIS_F4_PATH}/Include"
)

# search for system_stm32[XX]xx.c
find_file(
  CMSIS_F4_SYSTEM
  NAMES system_stm32f4xx.c
  PATHS "${CMSIS_F4_PATH}/Source/Templates"
  NO_DEFAULT_PATH
)
list(APPEND CMSIS_SOURCES "${CMSIS_F4_SYSTEM}")

set(STM_DEVICES_FOUND TRUE)

find_file(
  CMSIS_F4_F401xE_STARTUP
  NAMES startup_stm32f401xe.s startup_stm32f401xe.s
  PATHS "${CMSIS_F4_PATH}/Source/Templates/gcc"
  NO_DEFAULT_PATH
)
list(APPEND CMSIS_SOURCES "${CMSIS_F4_F401xE_STARTUP}")

add_library(CMSIS::STM32::F401xE INTERFACE IMPORTED)
target_link_libraries(
  CMSIS::STM32::F401xE INTERFACE CMSIS::STM32::F4 STM32::F401xE
)
target_sources(CMSIS::STM32::F401xE INTERFACE "${CMSIS_F4_F401xE_STARTUP}")
target_sources(CMSIS::STM32::F401xE INTERFACE "${CMSIS_F4_SYSTEM}")

add_library(CMSIS::STM32::F401RE INTERFACE IMPORTED)
target_link_libraries(CMSIS::STM32::F401RE INTERFACE CMSIS::STM32::F401xE)

set(CMSIS_STM32F4_FOUND TRUE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  CMSIS
  REQUIRED_VARS CMSIS_INCLUDE_DIRS CMSIS_SOURCES
  FOUND_VAR CMSIS_FOUND
  VERSION_VAR ${CMSIS_STM32F4_VERSION}
  HANDLE_COMPONENTS
)

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

set(CMSIS_STM32F4_VERSION ${CMSIS_F4_VERSION})
set(CMSIS_VERSION ${CMSIS_STM32F4_VERSION})

if(NOT (TARGET CMSIS::STM32::F4${}))
  message(STATUS "FindCMSIS: creating library CMSIS::STM32::F4${}")
  add_library(CMSIS::STM32::F4${} INTERFACE IMPORTED)
  # STM32::F4${} contains compile options and is define in <family>.cmake
  target_link_libraries(CMSIS::STM32::F4${} INTERFACE STM32::F4${})
  target_include_directories(
    CMSIS::STM32::F4${} INTERFACE "${CMSIS_F4_CORE_PATH}/Include"
  )
  target_include_directories(
    CMSIS::STM32::F4${} INTERFACE "${CMSIS_F4_PATH}/Include"
  )
endif()

# search for system_stm32[XX]xx.c
find_file(
  CMSIS_F4_SYSTEM
  NAMES system_stm32f4xx.c
  PATHS "${CMSIS_F4_PATH}/Source/Templates"
  NO_DEFAULT_PATH
)
list(APPEND CMSIS_SOURCES "${CMSIS_F4_SYSTEM}")

if(NOT CMSIS_F4_SYSTEM)
  message(VERBOSE "FindCMSIS: system_stm32f4xx.c for F4 has not been found")
  continue()
endif()

set(STM_DEVICES_FOUND TRUE)
foreach(DEVICE F401RE)
  message(TRACE "FindCMSIS: Iterating DEVICE ${DEVICE}")

  set(TYPE F401xE)
  string(TOLOWER ${DEVICE} DEVICE_L)
  string(TOLOWER ${TYPE} TYPE_L)

  get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
  if(NOT
     "ASM"
     IN_LIST
     languages
  )
    message(
      STATUS
        "FindCMSIS: Not generating target for startup file and linker script because ASM language is not enabled"
    )
    continue()
  endif()

  find_file(
    CMSIS_F4_${TYPE}_STARTUP
    NAMES startup_stm32${TYPE_L}.s startup_stm32${TYPE_L}.s
    PATHS "${CMSIS_F4_PATH}/Source/Templates/gcc"
    NO_DEFAULT_PATH
  )
  list(APPEND CMSIS_SOURCES "${CMSIS_F4_${TYPE}_STARTUP}")
  if(NOT CMSIS_F4_${TYPE}_STARTUP)
    set(STM_DEVICES_FOUND FALSE)
    message(
      VERBOSE
      "FindCMSIS: did not find file: startup_stm32${TYPE_L}.s or startup_stm32${TYPE_L}.s"
    )
    break()
  endif()

  if(NOT (TARGET CMSIS::STM32::${TYPE}${}))
    message(TRACE "FindCMSIS: creating library CMSIS::STM32::${TYPE}${}")
    add_library(CMSIS::STM32::${TYPE}${} INTERFACE IMPORTED)
    target_link_libraries(
      CMSIS::STM32::${TYPE}${} INTERFACE CMSIS::STM32::F4${} STM32::${TYPE}${}
    )
    target_sources(
      CMSIS::STM32::${TYPE}${} INTERFACE "${CMSIS_F4_${TYPE}_STARTUP}"
    )
    target_sources(CMSIS::STM32::${TYPE}${} INTERFACE "${CMSIS_F4_SYSTEM}")
  endif()

  add_library(CMSIS::STM32::${DEVICE}${} INTERFACE IMPORTED)
  target_link_libraries(
    CMSIS::STM32::${DEVICE}${} INTERFACE CMSIS::STM32::${TYPE}${}
  )
endforeach()

if(STM_DEVICES_FOUND)
  set(CMSIS_STM32F4_FOUND TRUE)
  message(DEBUG "CMSIS_STM32F4_FOUND TRUE")
else()
  set(CMSIS_STM32F4_FOUND FALSE)
  message(DEBUG "CMSIS_STM32F4_FOUND FALSE")
endif()

list(REMOVE_DUPLICATES CMSIS_INCLUDE_DIRS)
list(REMOVE_DUPLICATES CMSIS_SOURCES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  CMSIS
  REQUIRED_VARS CMSIS_INCLUDE_DIRS CMSIS_SOURCES
  FOUND_VAR CMSIS_FOUND
  VERSION_VAR CMSIS_VERSION
  HANDLE_COMPONENTS
)

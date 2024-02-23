# For information about why and how of this file:
# https://cmake.org/cmake/help/latest/command/find_package.html

if(NOT CMSIS_FIND_COMPONENTS)
  set(CMSIS_FIND_COMPONENTS ${STM32_SUPPORTED_FAMILIES_LONG_NAME})
endif()

list(REMOVE_DUPLICATES CMSIS_FIND_COMPONENTS)

if(NOT CMSIS_FIND_COMPONENTS_FAMILIES)
  set(CMSIS_FIND_COMPONENTS_FAMILIES ${STM32_SUPPORTED_FAMILIES_LONG_NAME})
endif()
message(STATUS "Search for CMSIS families: ${CMSIS_FIND_COMPONENTS_FAMILIES}")

include(stm32/devices)

function(stm32_get_chip_info CHIP)
  set(ARG_OPTIONS "")
  set(ARG_SINGLE FAMILY DEVICE TYPE)
  set(ARG_MULTIPLE "")
  cmake_parse_arguments(
    PARSE_ARGV
    1
    ARG
    "${ARG_OPTIONS}"
    "${ARG_SINGLE}"
    "${ARG_MULTIPLE}"
  )

  string(TOUPPER ${CHIP} CHIP)

  string(
    REGEX MATCH
          "^STM32([CFGHLMUW]P?[0-9BL])([0-9A-Z][0-9M][A-Z][0-9A-Z]).*$"
          CHIP
          ${CHIP}
  )

  if((NOT CMAKE_MATCH_1) OR (NOT CMAKE_MATCH_2))
    message(FATAL_ERROR "Unknown chip ${CHIP}")
  endif()

  set(STM32_FAMILY ${CMAKE_MATCH_1})
  set(STM32_DEVICE "${CMAKE_MATCH_1}${CMAKE_MATCH_2}")

  if(NOT (${STM32_FAMILY} IN_LIST STM32_SUPPORTED_FAMILIES_SHORT_NAME))
    message(FATAL_ERROR "Unsupported family ${STM32_FAMILY} for device ${CHIP}")
  endif()

  set(STM32_TYPE F401xE)

  if(ARG_FAMILY)
    set(${ARG_FAMILY} ${STM32_FAMILY} PARENT_SCOPE)
  endif()
  if(ARG_DEVICE)
    set(${ARG_DEVICE} ${STM32_DEVICE} PARENT_SCOPE)
  endif()
  if(ARG_TYPE)
    set(${ARG_TYPE} ${STM32_TYPE} PARENT_SCOPE)
  endif()
endfunction()

foreach(COMP ${CMSIS_FIND_COMPONENTS_FAMILIES})
  string(TOLOWER ${COMP} COMP_L)
  string(TOUPPER ${COMP} COMP)

  string(
    REGEX
      MATCH
      "^STM32([CFGHLMUW]P?[0-9BL])([0-9A-Z][0-9M][A-Z][0-9A-Z])?_?(M0PLUS|M4|M7)?.*$"
      COMP
      ${COMP}
  )
  # CMAKE_MATCH_<n> contains n'th subexpression CMAKE_MATCH_0 contains full
  # match

  set(FAMILY ${CMAKE_MATCH_1})
  stm32_get_devices_by_family(STM_DEVICES FAMILY ${FAMILY})
  message(
    TRACE
    "FindCMSIS: family only match for COMP ${COMP}, STM_DEVICES is ${STM_DEVICES}"
  )

  string(TOLOWER ${FAMILY} FAMILY_L)

  if((NOT STM32_CMSIS_${FAMILY}_PATH) AND (NOT STM32_CUBE_${FAMILY}_PATH)
     AND (DEFINED ENV{STM32_CUBE_${FAMILY}_PATH})
  )
    # try to set path from environment variable. Note it could be ...-NOT-FOUND
    # and it's fine
    set(STM32_CUBE_${FAMILY}_PATH $ENV{STM32_CUBE_${FAMILY}_PATH}
        CACHE PATH "Path to STM32Cube${FAMILY}"
    )
    message(
      STATUS
        "ENV STM32_CUBE_${FAMILY}_PATH specified, using STM32_CUBE_${FAMILY}_PATH: ${STM32_CUBE_${FAMILY}_PATH}"
    )
  endif()

  if((NOT STM32_CMSIS_${FAMILY}_PATH) AND (NOT STM32_CUBE_${FAMILY}_PATH))
    set(STM32_CUBE_${FAMILY}_PATH /opt/STM32Cube${FAMILY}
        CACHE PATH "Path to STM32Cube${FAMILY}"
    )
    message(
      STATUS
        "Neither STM32_CUBE_${FAMILY}_PATH nor STM32_CMSIS_${FAMILY}_PATH specified using default STM32_CUBE_${FAMILY}_PATH: ${STM32_CUBE_${FAMILY}_PATH}"
    )
  endif()

  # search for Include/cmsis_gcc.h
  find_path(
    CMSIS_${FAMILY}_CORE_PATH
    NAMES Include/cmsis_gcc.h
    PATHS "${STM32_CMSIS_PATH}" "${STM32_CUBE_${FAMILY}_PATH}/Drivers/CMSIS"
    NO_DEFAULT_PATH
  )
  if(NOT CMSIS_${FAMILY}_CORE_PATH)
    message(VERBOSE "FindCMSIS: cmsis_gcc.h for ${FAMILY} has not been found")
    continue()
  endif()

  # search for Include/stm32[XX]xx.h
  find_path(
    CMSIS_${FAMILY}_PATH
    NAMES Include/stm32${FAMILY_L}xx.h
    PATHS
      "${STM32_CMSIS_${FAMILY}_PATH}"
      "${STM32_CUBE_${FAMILY}_PATH}/Drivers/CMSIS/Device/ST/STM32${FAMILY}xx"
    NO_DEFAULT_PATH
  )
  if(NOT CMSIS_${FAMILY}_PATH)
    message(VERBOSE
            "FindCMSIS: stm32${FAMILY_L}xx.h for ${FAMILY} has not been found"
    )
    continue()
  endif()
  list(
    APPEND
    CMSIS_INCLUDE_DIRS
    "${CMSIS_${FAMILY}_CORE_PATH}/Include"
    "${CMSIS_${FAMILY}_PATH}/Include"
  )

  if(NOT CMSIS_${FAMILY}_VERSION)
    find_file(
      CMSIS_${FAMILY}_PDSC
      NAMES ARM.CMSIS.pdsc
      PATHS "${CMSIS_${FAMILY}_CORE_PATH}"
      NO_DEFAULT_PATH
    )
    if(NOT CMSIS_${FAMILY}_PDSC)
      set(CMSIS_${FAMILY}_VERSION "0.0.0")
    else()
      file(
        STRINGS "${CMSIS_${FAMILY}_PDSC}" VERSION_STRINGS
        REGEX
          "<release version=\"([0-9]*\\.[0-9]*\\.[0-9]*)\" date=\"[0-9]+\\-[0-9]+\\-[0-9]+\">"
      )
      list(
        GET
        VERSION_STRINGS
        0
        STR
      )
      string(
        REGEX
          MATCH
          "<release version=\"([0-9]*)\\.([0-9]*)\\.([0-9]*)\" date=\"[0-9]+\\-[0-9]+\\-[0-9]+\">"
          MATCHED
          ${STR}
      )
      set(CMSIS_${FAMILY}_VERSION
          "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}"
          CACHE INTERNAL "CMSIS STM32${FAMILY} version"
      )
    endif()
  endif()

  set(CMSIS_${COMP}_VERSION ${CMSIS_${FAMILY}_VERSION})
  set(CMSIS_VERSION ${CMSIS_${COMP}_VERSION})

  if(NOT (TARGET CMSIS::STM32::${FAMILY}${}))
    message(TRACE "FindCMSIS: creating library CMSIS::STM32::${FAMILY}${}")
    add_library(CMSIS::STM32::${FAMILY}${} INTERFACE IMPORTED)
    # STM32::${FAMILY}${} contains compile options and is define in
    # <family>.cmake
    target_link_libraries(
      CMSIS::STM32::${FAMILY}${} INTERFACE STM32::${FAMILY}${}
    )
    target_include_directories(
      CMSIS::STM32::${FAMILY}${}
      INTERFACE "${CMSIS_${FAMILY}_CORE_PATH}/Include"
    )
    target_include_directories(
      CMSIS::STM32::${FAMILY}${} INTERFACE "${CMSIS_${FAMILY}_PATH}/Include"
    )
  endif()

  # search for system_stm32[XX]xx.c
  find_file(
    CMSIS_${FAMILY}_SYSTEM
    NAMES system_stm32${FAMILY_L}xx.c
    PATHS "${CMSIS_${FAMILY}_PATH}/Source/Templates"
    NO_DEFAULT_PATH
  )
  list(APPEND CMSIS_SOURCES "${CMSIS_${FAMILY}_SYSTEM}")

  if(NOT CMSIS_${FAMILY}_SYSTEM)
    message(
      VERBOSE
      "FindCMSIS: system_stm32${FAMILY_L}xx.c for ${FAMILY} has not been found"
    )
    continue()
  endif()

  set(STM_DEVICES_FOUND TRUE)
  foreach(DEVICE ${STM_DEVICES})
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
      CMSIS_${FAMILY}_${TYPE}_STARTUP
      NAMES startup_stm32${TYPE_L}.s startup_stm32${TYPE_L}.s
      PATHS "${CMSIS_${FAMILY}_PATH}/Source/Templates/gcc"
      NO_DEFAULT_PATH
    )
    list(APPEND CMSIS_SOURCES "${CMSIS_${FAMILY}_${TYPE}_STARTUP}")
    if(NOT CMSIS_${FAMILY}_${TYPE}_STARTUP)
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
        CMSIS::STM32::${TYPE}${} INTERFACE CMSIS::STM32::${FAMILY}${}
                                           STM32::${TYPE}${}
      )
      target_sources(
        CMSIS::STM32::${TYPE}${} INTERFACE "${CMSIS_${FAMILY}_${TYPE}_STARTUP}"
      )
      target_sources(
        CMSIS::STM32::${TYPE}${} INTERFACE "${CMSIS_${FAMILY}_SYSTEM}"
      )
    endif()

    add_library(CMSIS::STM32::${DEVICE}${} INTERFACE IMPORTED)
    target_link_libraries(
      CMSIS::STM32::${DEVICE}${} INTERFACE CMSIS::STM32::${TYPE}${}
    )
  endforeach()

  if(STM_DEVICES_FOUND)
    set(CMSIS_${COMP}_FOUND TRUE)
    message(DEBUG "CMSIS_${COMP}_FOUND TRUE")
  else()
    set(CMSIS_${COMP}_FOUND FALSE)
    message(DEBUG "CMSIS_${COMP}_FOUND FALSE")
  endif()

  list(REMOVE_DUPLICATES CMSIS_INCLUDE_DIRS)
  list(REMOVE_DUPLICATES CMSIS_SOURCES)
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  CMSIS
  REQUIRED_VARS CMSIS_INCLUDE_DIRS CMSIS_SOURCES
  FOUND_VAR CMSIS_FOUND
  VERSION_VAR CMSIS_VERSION
  HANDLE_COMPONENTS
)

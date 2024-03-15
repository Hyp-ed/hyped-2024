# Portions of this code are derived from
# https://github.com/ObKo/stm32-cmake/tree/master?tab=MIT-1-ov-file#readme
# available under the MIT License. You may obtain a copy of the MIT License at
#
# https://opensource.org/licenses/MIT

# This function gets a list of hal_driver using a given prefix and suffix
#
# out_list_hal_drivers   list of hal_drivers found hal_drivers_path       path
# to the hal's drivers hal_driver_type        hal_driver type to find
# (hal/ll/ex)
function(
  get_list_hal_drivers
  out_list_hal_drivers
  hal_drivers_path
  hal_driver_type
)
  # The pattern to retrieve a driver from a file name depends on the
  # hal_driver_type field
  if(${hal_driver_type} STREQUAL "hal" OR ${hal_driver_type} STREQUAL "ll")
    # This regex match and capture a driver type (stm32xx_hal_(rcc).c or
    # stm32xx_ll_(rcc).c => catches rcc)
    set(file_pattern ".+_${hal_driver_type}_([a-z0-9]+)\\.c$")
  elseif(${hal_driver_type} STREQUAL "ex")
    # This regex match and capture a driver type (stm32xx_hal_(rcc)_ex.c =>
    # catches rcc)
    set(file_pattern ".+_hal_([a-z0-9]+)_ex\\.c$")
  else()
    message(
      FATAL_ERROR
        "the inputed hal_driver_type(${hal_driver_type}) is not valid."
    )
  endif()

  # Retrieving all the .c files from hal_drivers_path
  file(GLOB filtered_files RELATIVE "${hal_drivers_path}/Src"
       "${hal_drivers_path}/Src/*.c"
  )
  # For all matched .c files keep only those with a driver name pattern (e.g.
  # stm32xx_hal_rcc.c)
  list(
    FILTER
    filtered_files
    INCLUDE
    REGEX
    ${file_pattern}
  )
  # From the files names keep only the driver type part using the regex
  # (stm32xx_hal_(rcc).c or stm32xx_ll_(rcc).c => catches rcc)
  list(TRANSFORM filtered_files REPLACE ${file_pattern} "\\1")
  # Making a return by reference by seting the output variable to PARENT_SCOPE
  set(${out_list_hal_drivers} ${filtered_files} PARENT_SCOPE)
endfunction()

# ##############################################################################
# Checking the parameters provided to the find_package(HAL ...) call The
# expected parameters are families and or drivers in *any orders* Families are
# valid if on the list of known families. Drivers are valid if on the list of
# valid driver of any family. For this reason the requested families must be
# processed in two steps - Step 1 : Checking all the requested families - Step 2
# : Generating all the valid drivers from requested families - Step 3 : Checking
# the other requested components (Expected to be drivers)
# ##############################################################################
# Step 1 : Checking all the requested families
foreach(COMP ${HAL_FIND_COMPONENTS})
  string(TOUPPER ${COMP} COMP_U)
  string(
    REGEX
      MATCH
      "^STM32([CFGHLMUW]P?[0-9BL])([0-9A-Z][0-9M][A-Z][0-9A-Z])?_?(M0PLUS|M4|M7)?.*$"
      COMP_U
      ${COMP_U}
  )
  if(CMAKE_MATCH_1) # Matches the family part of the provided STM32<FAMILY>[..]
                    # component
    list(APPEND HAL_FIND_COMPONENTS_FAMILIES ${COMP})
    message(TRACE
            "FindHAL: append COMP ${COMP} to HAL_FIND_COMPONENTS_FAMILIES"
    )
  else()
    list(APPEND HAL_FIND_COMPONENTS_UNHANDLED ${COMP})
  endif()
endforeach()

# Find the path to the HAL library
find_path(
  HAL_F4_PATH
  NAMES Inc/stm32f4xx_hal.h
  PATHS "${STM32_HAL_F4_PATH}"
        "${STM32_CUBE_F4_PATH}/Drivers/STM32F4xx_HAL_Driver"
  NO_DEFAULT_PATH
)
set(HAL_STM32F4_FOUND TRUE)

get_list_hal_drivers(HAL_DRIVERS_F4 ${HAL_F4_PATH} "hal")
get_list_hal_drivers(HAL_EX_DRIVERS_F4 ${HAL_F4_PATH} "ex")
get_list_hal_drivers(HAL_LL_DRIVERS_F4 ${HAL_F4_PATH} "ll")
list(APPEND HAL_DRIVERS ${HAL_DRIVERS_F4})
list(APPEND HAL_LL_DRIVERS ${HAL_LL_DRIVERS_F4})

list(REMOVE_DUPLICATES HAL_DRIVERS)
list(REMOVE_DUPLICATES HAL_LL_DRIVERS)

# Find the drivers
foreach(COMP ${HAL_FIND_COMPONENTS_UNHANDLED})
  string(TOLOWER ${COMP} COMP_L)

  if(${COMP_L} IN_LIST HAL_DRIVERS)
    list(APPEND HAL_FIND_COMPONENTS_DRIVERS ${COMP})
    continue()
  endif()
  string(
    REGEX
    REPLACE "^ll_"
            ""
            COMP_L
            ${COMP_L}
  )
  message(FATAL_ERROR "FindHAL: unknown HAL component: ${COMP}")
endforeach()

list(REMOVE_DUPLICATES HAL_FIND_COMPONENTS_FAMILIES)

message(STATUS "Search for HAL families: ${HAL_FIND_COMPONENTS_FAMILIES}")
message(STATUS "Search for HAL drivers: ${HAL_FIND_COMPONENTS_DRIVERS}")

string(TOUPPER STM32F4 COMP_U)

find_path(
  HAL_F4_PATH
  NAMES Inc/stm32f4xx_hal.h
  PATHS "${STM32_HAL_F4_PATH}"
        "${STM32_CUBE_F4_PATH}/Drivers/STM32F4xx_HAL_Driver"
  NO_DEFAULT_PATH
)
find_path(
  HAL_F4_INCLUDE
  NAMES stm32f4xx_hal.h
  PATHS "${HAL_F4_PATH}/Inc"
  NO_DEFAULT_PATH
)
find_file(
  HAL_F4_SOURCE
  NAMES stm32f4xx_hal.c
  PATHS "${HAL_F4_PATH}/Src"
  NO_DEFAULT_PATH
)

message(STATUS "FindHAL: creating library HAL::STM32::F4")
add_library(HAL::STM32::F4 INTERFACE IMPORTED)
target_link_libraries(HAL::STM32::F4 INTERFACE STM32::F4 CMSIS::STM32::F4)
target_include_directories(HAL::STM32::F4 INTERFACE "${HAL_F4_INCLUDE}")
target_sources(HAL::STM32::F4 INTERFACE "${HAL_F4_SOURCE}")

foreach(DRV_COMP ${HAL_FIND_COMPONENTS_DRIVERS})
  string(TOLOWER ${DRV_COMP} DRV_L)
  string(TOUPPER ${DRV_COMP} DRV)

  if(NOT (DRV_L IN_LIST HAL_DRIVERS_F4))
    message(FATAL_ERROR "FindHAL: unknown HAL driver: ${DRV_COMP}")
  endif()

  find_file(
    HAL_F4_${DRV}_SOURCE
    NAMES stm32f4xx_hal_${DRV_L}.c
    PATHS "${HAL_F4_PATH}/Src"
    NO_DEFAULT_PATH
  )
  list(APPEND HAL_F4_SOURCES "${HAL_F4_${DRV}_SOURCE}")

  set(HAL_${DRV_COMP}_FOUND TRUE)

  message(STATUS "FindHAL: creating library HAL::STM32::F4::${DRV}")
  add_library(HAL::STM32::F4::${DRV} INTERFACE IMPORTED)
  target_link_libraries(HAL::STM32::F4::${DRV} INTERFACE HAL::STM32::F4)
  target_sources(HAL::STM32::F4::${DRV} INTERFACE "${HAL_F4_${DRV}_SOURCE}")
endforeach()

set(HAL_STM32F4_FOUND TRUE)
list(APPEND HAL_INCLUDE_DIRS "${HAL_F4_INCLUDE}")
list(APPEND HAL_SOURCES "${HAL_F4_SOURCES}")

list(REMOVE_DUPLICATES HAL_INCLUDE_DIRS)
list(REMOVE_DUPLICATES HAL_SOURCES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  HAL
  REQUIRED_VARS HAL_INCLUDE_DIRS HAL_SOURCES
  FOUND_VAR HAL_FOUND
  HANDLE_COMPONENTS
)

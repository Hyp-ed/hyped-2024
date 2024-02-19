function(stm32_util_create_family_targets FAMILY)
  set(CORES ${ARGN})
  list(LENGTH CORES NUM_CORES)
  if(${NUM_CORES} EQUAL 0)
    set(CORE "")
    set(CORE_C "")
  elseif(${NUM_CORES} EQUAL 1)
    set(CORE "_${CORES}")
    set(CORE_C "::${CORES}")
  else()
    message(
      FATAL_ERROR "Expected at most one core for family ${FAMILY}: ${CORES}"
    )
  endif()

  if(NOT (TARGET STM32::${FAMILY}${CORE_C}))
    add_library(STM32::${FAMILY}${CORE_C} INTERFACE IMPORTED)
    # Set compiler flags for target -Wall: all warnings activated
    # -ffunction-sections -fdata-sections: remove unused code
    target_compile_options(
      STM32::${FAMILY}${CORE_C}
      INTERFACE -mthumb
                -Wall
                -ffunction-sections
                -fdata-sections
    )
    # Set linker flags -mthumb: Generate thumb code -Wl,--gc-sections: Remove
    # unused code
    target_link_options(
      STM32::${FAMILY}${CORE_C}
      INTERFACE
      -mthumb
      -Wl,--gc-sections
    )
    target_compile_definitions(
      STM32::${FAMILY}${CORE_C} INTERFACE STM32${FAMILY}
    )
  endif()
  foreach(TYPE ${STM32_${FAMILY}_TYPES})
    if(NOT (TARGET STM32::${TYPE}${CORE_C}))
      add_library(STM32::${TYPE}${CORE_C} INTERFACE IMPORTED)
      target_link_libraries(
        STM32::${TYPE}${CORE_C} INTERFACE STM32::${FAMILY}${CORE_C}
      )
      target_compile_definitions(STM32::${TYPE}${CORE_C} INTERFACE STM32${TYPE})
    endif()
  endforeach()
endfunction()

include(FetchContent)

function(stm32_fetch_cmsis FAMILY)
  if(NOT STM32_CMSIS_PATH)
    FetchContent_MakeAvailable(STM32-CMSIS)
    set(STM32_CMSIS_PATH ${stm32-cmsis_SOURCE_DIR} PARENT_SCOPE)
  else()
    message(INFO "STM32_CMSIS_PATH specified, skipping fetch for STM32-CMSIS")
  endif()

  if(STM32_USE_CMSIS_FROM_CUBE_${FAMILY})
    stm32_fetch_cube(${FAMILY})
    message(STATUS "Cube fetched for ${FAMILY} at ${STM32_CUBE_${FAMILY}_PATH}")
    set(STM32_CMSIS_${FAMILY}_PATH
        ${STM32_CUBE_${FAMILY}_PATH}/Drivers/CMSIS/Device/ST/STM32${FAMILY}xx
        PARENT_SCOPE
    )
  else()
    set(CMSIS_NAME STM32-CMSIS-${FAMILY})
    string(TOLOWER ${CMSIS_NAME} CMSIS_NAME_L)

    if(STM32_CMSIS_${FAMILY}_PATH)
      message(
        INFO
        "STM32_CMSIS_${FAMILY}_PATH specified, skipping fetch for ${CMSIS_NAME}"
      )
      continue()
    endif()

    FetchContent_MakeAvailable(${CMSIS_NAME})
    set(STM32_CMSIS_${FAMILY}_PATH ${${CMSIS_NAME_L}_SOURCE_DIR} PARENT_SCOPE)
  endif()
endfunction()

function(stm32_fetch_hal FAMILY)
  if(STM32_USE_HAL_FROM_CUBE_${FAMILY})
    stm32_fetch_cube(${FAMILY})
    message(STATUS "Cube fetched for ${FAMILY} at ${STM32_CUBE_${FAMILY}_PATH}")
    set(STM32_HAL_${FAMILY}_PATH
        ${STM32_CUBE_${FAMILY}_PATH}/Drivers/STM32${FAMILY}xx_HAL_Driver
        PARENT_SCOPE
    )
  else()
    set(HAL_NAME STM32-HAL-${FAMILY})
    string(TOLOWER ${HAL_NAME} HAL_NAME_L)

    if(STM32_HAL_${FAMILY}_PATH)
      message(
        INFO
        "STM32_HAL_${FAMILY}_PATH specified, skipping fetch for ${HAL_NAME}"
      )
      continue()
    endif()

    FetchContent_MakeAvailable(${HAL_NAME})
    set(STM32_HAL_${FAMILY}_PATH ${${HAL_NAME_L}_SOURCE_DIR} PARENT_SCOPE)
  endif()
endfunction()

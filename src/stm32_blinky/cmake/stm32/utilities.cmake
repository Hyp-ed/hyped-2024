function(stm32_util_create_family_targets FAMILY)

  if(NOT (TARGET STM32::${FAMILY}))
    add_library(STM32::${FAMILY} INTERFACE IMPORTED)
    # Set compiler flags for target -Wall: all warnings activated
    # -ffunction-sections -fdata-sections: remove unused code
    target_compile_options(
      STM32::${FAMILY}
      INTERFACE -mthumb
                -Wall
                -ffunction-sections
                -fdata-sections
    )
    # Set linker flags -mthumb: Generate thumb code -Wl,--gc-sections: Remove
    # unused code
    target_link_options(
      STM32::${FAMILY}
      INTERFACE
      -mthumb
      -Wl,--gc-sections
    )
    target_compile_definitions(STM32::${FAMILY} INTERFACE STM32${FAMILY})
  endif()
  foreach(TYPE ${STM32_${FAMILY}_TYPES})
    if(NOT (TARGET STM32::${TYPE}))
      add_library(STM32::${TYPE} INTERFACE IMPORTED)
      target_link_libraries(STM32::${TYPE} INTERFACE STM32::${FAMILY})
      target_compile_definitions(STM32::${TYPE} INTERFACE STM32${TYPE})
    endif()
  endforeach()
endfunction()

include(FetchContent)

function(stm32_fetch_cmsis FAMILY)
  FetchContent_MakeAvailable(STM32-CMSIS)
  set(STM32_CMSIS_PATH ${stm32-cmsis_SOURCE_DIR} PARENT_SCOPE)
  set(CMSIS_NAME STM32-CMSIS-${FAMILY})
  string(TOLOWER ${CMSIS_NAME} CMSIS_NAME_L)
  FetchContent_MakeAvailable(${CMSIS_NAME})
  set(STM32_CMSIS_${FAMILY}_PATH ${${CMSIS_NAME_L}_SOURCE_DIR} PARENT_SCOPE)
endfunction()

function(stm32_fetch_hal FAMILY)
  set(HAL_NAME STM32-HAL-${FAMILY})
  string(TOLOWER ${HAL_NAME} HAL_NAME_L)
  FetchContent_MakeAvailable(${HAL_NAME})
  set(STM32_HAL_${FAMILY}_PATH ${${HAL_NAME_L}_SOURCE_DIR} PARENT_SCOPE)
endfunction()

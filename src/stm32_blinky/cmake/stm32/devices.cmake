set(STM32_ALL_DEVICES F401RE F767ZI)

# Store a list of devices into a given STM_DEVICES list. You can also specify
# multiple device families. Examples: Get list of all devices for H7 family:
# stm32_get_devices_by_family(STM_DEVICES FAMILY H7) Get list of all devices:
# stm32_get_devices_by_family(STM_DEVICES)
function(stm32_get_devices_by_family STM_DEVICES)
  # Specify keywords for argument parsing here
  set(ARG_OPTIONS "")
  set(ARG_SINGLE "")
  set(ARG_MULTIPLE FAMILY)

  # Parse arguments. Multiple families can be specified and will be stored in
  # ARG_<KeywordName>
  cmake_parse_arguments(
    PARSE_ARGV
    1
    ARG
    "${ARG_OPTIONS}"
    "${ARG_SINGLE}"
    "${ARG_MULTIPLE}"
  )

  # Build a list of families by filtering the whole list with the specified
  # families
  if(ARG_FAMILY)
    set(RESULTING_DEV_LIST "")
    foreach(FAMILY ${ARG_FAMILY})
      set(STM_DEVICE_LIST ${STM32_ALL_DEVICES})
      list(
        FILTER
        STM_DEVICE_LIST
        INCLUDE
        REGEX
        "^${FAMILY}"
      )
      list(APPEND RESULTING_DEV_LIST ${STM_DEVICE_LIST})
      if(NOT STM_DEVICE_LIST)
        message(WARNING "No devices found for given family ${FAMILY}")
      endif()
    endforeach()
  else()
    # No family argument, so get list of all devices
    set(RESULTING_DEV_LIST ${STM32_ALL_DEVICES})
  endif()

  set(${STM_DEVICES} ${RESULTING_DEV_LIST} PARENT_SCOPE)
endfunction()

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
  stm32_dev_parser_check()

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

# Print the devices for a given family. You can also specify multiple device
# families. Example usage: Print devices for H7 family:
# stm32_print_devices_by_family(FAMILY H7) Print all devices:
# stm32_print_devices_by_family()
function(stm32_print_devices_by_family)
  # Specify keywords for argument parsing here
  set(ARG_OPTIONS "")
  set(ARG_SINGLE "")
  set(ARG_MULTIPLE FAMILY)

  # Parse arguments. Multiple families can be specified and will be stored in
  # ARG_<KeywordName>
  cmake_parse_arguments(
    PARSE_ARGV
    0
    ARG
    "${ARG_OPTIONS}"
    "${ARG_SINGLE}"
    "${ARG_MULTIPLE}"
  )
  stm32_dev_parser_check()

  if(ARG_FAMILY)
    # print devices one family per line
    foreach(FAMILY ${ARG_FAMILY})
      stm32_get_devices_by_family(STM_DEVICES FAMILY ${FAMILY})
      stm32_pretty_print_dev_list(${FAMILY} "${STM_DEVICES}")
    endforeach()
  else()
    # print all devices
    stm32_get_devices_by_family(STM_DEVICES)
    stm32_pretty_print_dev_list("all" "${STM_DEVICES}")
  endif()

endfunction()

# The arguments checked in this macro are filled by cmake_parse_argument
macro(stm32_dev_parser_check)
  # contains unexpected arguments (unknown keywords beofre ARG_MULTIPLE)
  if(ARG_UNPARSED_ARGUMENTS)
    message(
      WARNING "Unknown keyword(s) ${ARG_UNPARSED_ARGUMENTS} will be ignored"
    )
  endif()
  # is populated if ARG_SINGLE or ARG_MULTIPLE is used without values
  if(ARG_KEYWORDS_MISSING_VALUES)
    message(FATAL_ERROR "Keyword ${ARG_KEYWORDS_MISSING_VALUES} expects values")
  endif()
endmacro()

# Pretty printer to limit amount of list entries printed per line
macro(stm32_pretty_print_dev_list FAMILIES STM_DEVICES)
  if(${FAMILIES} STREQUAL "all")
    message(STATUS "Devices for all families")
  else()
    message(STATUS "Devices for ${FAMILIES} family")
  endif()
  set(TMP_LIST "")
  foreach(STM_DEVICE ${STM_DEVICES})
    list(APPEND TMP_LIST ${STM_DEVICE})
    list(LENGTH TMP_LIST CURR_LEN)
    if(CURR_LEN EQUAL 10)
      message(STATUS "${TMP_LIST}")
      set(TMP_LIST "")
    endif()
  endforeach()
  if(TMP_LIST)
    message(STATUS "${TMP_LIST}")
  endif()
endmacro()

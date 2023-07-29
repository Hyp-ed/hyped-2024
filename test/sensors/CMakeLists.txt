set(target hyped_test_sensors)
file(GLOB code "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${target} ${code})
target_include_directories(${target}
    PRIVATE
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(${target}
    GTest::gtest_main
    hyped_sensors
    hyped_utils
    hyped_core
)
gtest_discover_tests(${target})

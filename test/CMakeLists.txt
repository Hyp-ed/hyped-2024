include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_subdirectory(utils)
add_subdirectory(navigation)
add_subdirectory(core)
add_subdirectory(sensors)
add_subdirectory(state_machine)
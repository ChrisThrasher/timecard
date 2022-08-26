if(NOT BUILD_TESTING)
    return()
endif()

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.1.0)
FetchContent_MakeAvailable(Catch2)
set_target_properties(Catch2 PROPERTIES COMPILE_OPTIONS "")
include(Catch)

add_executable(test-timecard-core UnitTests.cpp)
target_link_libraries(test-timecard-core PRIVATE timecard-core Catch2::Catch2WithMain)
catch_discover_tests(test-timecard-core)

add_test(NAME test-help-flag COMMAND timecard --help)
set_property(TEST test-help-flag PROPERTY PASS_REGULAR_EXPRESSION Usage.*)

add_test(NAME test-version-flag COMMAND timecard --version)
set_property(TEST test-version-flag PROPERTY PASS_REGULAR_EXPRESSION v${timecard_VERSION})

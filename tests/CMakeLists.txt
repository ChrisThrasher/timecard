if(NOT BUILD_TESTING)
    return()
endif()

find_package(Catch2 3.1.0 REQUIRED)

add_executable(test-timecard-core
    CalculateDurations.cpp
    DurationMap.cpp
    FormatDurations.cpp
    ParseTimepoint.cpp
    Timecard.cpp)
target_link_libraries(test-timecard-core PRIVATE timecard-core Catch2::Catch2WithMain)
catch_discover_tests(test-timecard-core)

add_test(NAME test-help-flag COMMAND timecard --help)
set_property(TEST test-help-flag PROPERTY PASS_REGULAR_EXPRESSION Usage.*)

add_test(NAME test-version-flag COMMAND timecard --version)
set_property(TEST test-version-flag PROPERTY PASS_REGULAR_EXPRESSION v${timecard_VERSION})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Catch2 3.3.1 REQUIRED)
include(Catch)

add_executable(test-timecard
    CalculateDurations.cpp
    FormatDurations.cpp
    ParseTimepoint.cpp
    Timecard.cpp)
target_link_libraries(test-timecard PRIVATE libtimecard Catch2::Catch2WithMain)
catch_discover_tests(test-timecard)

add_test(NAME test-help-flag COMMAND timecard --help)
set_property(TEST test-help-flag PROPERTY PASS_REGULAR_EXPRESSION Usage.*)

add_test(NAME test-version-flag COMMAND timecard --version)
set_property(TEST test-version-flag PROPERTY PASS_REGULAR_EXPRESSION v${timecard_VERSION})

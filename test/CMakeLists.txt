FetchContent_Declare(
    gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG master
)
FetchContent_GetProperties(gtest)
FetchContent_Populate(gtest)
add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR} EXCLUDE_FROM_ALL)

add_executable(unit_tests UnitTests.cpp)
target_link_libraries(unit_tests PRIVATE core gtest_main)
add_test(NAME unit_tests COMMAND unit_tests)

add_test(NAME flag--help COMMAND timecard --help)
set_property(TEST flag--help PROPERTY PASS_REGULAR_EXPRESSION "Usage.*")

add_test(NAME flag--version COMMAND timecard --version)
set_property(TEST flag--version PROPERTY PASS_REGULAR_EXPRESSION "${GIT_VERSION}")

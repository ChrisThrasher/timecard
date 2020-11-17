FetchContent_Declare(
    gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG master
)
FetchContent_GetProperties(gtest)
FetchContent_Populate(gtest)
add_subdirectory(${gtest_SOURCE_DIR} ${gtest_BINARY_DIR} EXCLUDE_FROM_ALL)

add_executable(unit_tests UnitTests.cpp)
target_include_directories(unit_tests PRIVATE ../include)
target_link_libraries(unit_tests PRIVATE gtest)
target_compile_options(unit_tests PRIVATE -Werror -Wall -Wextra)
add_test(NAME unit_tests COMMAND unit_tests)

add_test(NAME flag-h COMMAND timecard -h)
set_property(TEST flag-h PROPERTY PASS_REGULAR_EXPRESSION "Usage.*")

add_test(NAME flag--help COMMAND timecard --help)
set_property(TEST flag--help PROPERTY PASS_REGULAR_EXPRESSION "Usage.*")

add_test(NAME flag-v COMMAND timecard -v)
set_property(TEST flag-v PROPERTY PASS_REGULAR_EXPRESSION "${GIT_VERSION}")

add_test(NAME flag--version COMMAND timecard --version)
set_property(TEST flag--version PROPERTY PASS_REGULAR_EXPRESSION "${GIT_VERSION}")

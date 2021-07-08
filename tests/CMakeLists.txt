set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG release-1.11.0)
FetchContent_MakeAvailable(gtest)

add_executable(unit_tests UnitTests.cpp)
target_link_libraries(unit_tests PRIVATE libtimecard GTest::gtest_main)
add_test(NAME unit_tests COMMAND unit_tests)

add_test(NAME flag--help COMMAND timecard --help)
set_property(TEST flag--help PROPERTY PASS_REGULAR_EXPRESSION "Usage.*")

add_test(NAME flag--version COMMAND timecard --version)
set_property(TEST flag--version PROPERTY PASS_REGULAR_EXPRESSION "${GIT_VERSION}")

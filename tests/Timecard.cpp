#include <catch2/catch_test_macros.hpp>

TEST_CASE("Program version")
{
#ifndef TIMECARD_VERSION
    FAIL("TIMECARD_VERSION symbol not defined");
#endif
    REQUIRE_FALSE(std::string(TIMECARD_VERSION).empty());
    CHECK('v' == TIMECARD_VERSION[0]);
}

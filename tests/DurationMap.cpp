#include <timecard/DurationMap.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("DurationMap")
{
    using namespace std::chrono_literals;
    CHECK(0h == DurationMap::mapped_type(0));
    CHECK(1h == DurationMap::mapped_type(1));
    CHECK(5h == DurationMap::mapped_type(5));
    CHECK(10h == DurationMap::mapped_type(10));
    CHECK(100h == DurationMap::mapped_type(100));
}

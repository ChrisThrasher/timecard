#include <timecard/ParseTimepoint.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("parse_timepoint")
{
    SECTION("Garbage timepoints")
    {
        CHECK_THROWS(parse_timepoint(""));
        CHECK_THROWS(parse_timepoint(" "));
        CHECK_THROWS(parse_timepoint("am"));
        CHECK_THROWS(parse_timepoint("pm"));
        CHECK_THROWS(parse_timepoint("asdf"));
        CHECK_THROWS(parse_timepoint("just a plain ole string"));
        CHECK_THROWS(parse_timepoint("af13"));
        CHECK_THROWS(parse_timepoint("1200tns"));
    }

    SECTION("Out of range hours")
    {
        CHECK_THROWS(parse_timepoint("0:00am"));
        CHECK_THROWS(parse_timepoint("00:00am"));
        CHECK_THROWS(parse_timepoint("13:00am"));
        CHECK_THROWS(parse_timepoint("13:00pm"));
        CHECK_THROWS(parse_timepoint("20:00pm"));
        CHECK_THROWS(parse_timepoint("25:00pm"));
    }

    SECTION("Out of range minutes")
    {
        CHECK_THROWS(parse_timepoint("1:60am"));
        CHECK_THROWS(parse_timepoint("4:90am"));
        CHECK_THROWS(parse_timepoint("12:61pm"));
    }

    SECTION("Valid AM times")
    {
        using namespace std::chrono_literals;
        CHECK(0h + 0min == parse_timepoint("12:00am"));
        CHECK(0h + 30min == parse_timepoint("12:30am"));
        CHECK(2h + 15min == parse_timepoint("2:15am"));
        CHECK(4h + 20min == parse_timepoint("4:20am"));
        CHECK(9h + 41min == parse_timepoint("9:41am"));
        CHECK(7h + 1min == parse_timepoint("7:01am"));
        CHECK(11h + 0min == parse_timepoint("11:00am"));
        CHECK(11h + 30min == parse_timepoint("11:30am"));
        CHECK(11h + 59min == parse_timepoint("11:59am"));
    }

    SECTION("Valid PM times")
    {
        using namespace std::chrono_literals;
        CHECK(12h + 0min == parse_timepoint("12:00pm"));
        CHECK(12h + 30min == parse_timepoint("12:30pm"));
        CHECK(13h + 0min == parse_timepoint("1:00pm"));
        CHECK(14h + 0min == parse_timepoint("2:00pm"));
        CHECK(14h + 30min == parse_timepoint("2:30pm"));
        CHECK(16h + 6min == parse_timepoint("4:06pm"));
        CHECK(22h + 7min == parse_timepoint("10:07pm"));
        CHECK(23h + 59min == parse_timepoint("11:59pm"));
    }

    SECTION("Abbreviated times")
    {
        using namespace std::chrono_literals;
        CHECK(0h == parse_timepoint("12am"));
        CHECK(1h == parse_timepoint("1am"));
        CHECK(8h == parse_timepoint("8am"));
        CHECK(12h == parse_timepoint("12pm"));
        CHECK(13h == parse_timepoint("1pm"));
        CHECK(23h == parse_timepoint("11pm"));
    }
}

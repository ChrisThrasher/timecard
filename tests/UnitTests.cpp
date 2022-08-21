#include <timecard/CalculateDurations.hpp>
#include <timecard/DurationMap.hpp>
#include <timecard/FormatDurations.hpp>
#include <timecard/ParseTimepoint.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Program version")
{
#ifndef TIMECARD_VERSION
    FAIL("TIMECARD_VERSION symbol not defined");
#endif
    REQUIRE_FALSE(std::string(TIMECARD_VERSION).empty());
    CHECK('v' == TIMECARD_VERSION[0]);
}

TEST_CASE("Parse garbage timepoints")
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

TEST_CASE("Parse out of range hours")
{
    CHECK_THROWS(parse_timepoint("0:00am"));
    CHECK_THROWS(parse_timepoint("00:00am"));
    CHECK_THROWS(parse_timepoint("13:00am"));
    CHECK_THROWS(parse_timepoint("13:00pm"));
    CHECK_THROWS(parse_timepoint("20:00pm"));
    CHECK_THROWS(parse_timepoint("25:00pm"));
}

TEST_CASE("Parse out of range minutes")
{
    CHECK_THROWS(parse_timepoint("1:60am"));
    CHECK_THROWS(parse_timepoint("4:90am"));
    CHECK_THROWS(parse_timepoint("12:61pm"));
}

TEST_CASE("Parse valid AM times")
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

TEST_CASE("Parse valid PM times")
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

TEST_CASE("Parse abbreviated times")
{
    using namespace std::chrono_literals;
    CHECK(0h == parse_timepoint("12am"));
    CHECK(1h == parse_timepoint("1am"));
    CHECK(8h == parse_timepoint("8am"));
    CHECK(12h == parse_timepoint("12pm"));
    CHECK(13h == parse_timepoint("1pm"));
    CHECK(23h == parse_timepoint("11pm"));
}

TEST_CASE("DurationMap values")
{
    using namespace std::chrono_literals;
    CHECK(0h == DurationMap::mapped_type(0));
    CHECK(1h == DurationMap::mapped_type(1));
    CHECK(5h == DurationMap::mapped_type(5));
    CHECK(10h == DurationMap::mapped_type(10));
    CHECK(100h == DurationMap::mapped_type(100));
}

TEST_CASE("Too few durations to calculate")
{
    CHECK_THROWS(calculate_durations({}));
    CHECK_THROWS(calculate_durations({ "12:00pm" }));
    CHECK_THROWS(calculate_durations({ "12:00pm", "12:30pm" }));
}

TEST_CASE("Extra unparsed arguments")
{
    CHECK_THROWS(calculate_durations({ "1am", "so_far_so_good", "2:00am", "oh_no_an_extra_label" }));
    CHECK_THROWS(calculate_durations({ "2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "time_to_throw" }));
    CHECK_THROWS(calculate_durations({ "2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "-" }));
    CHECK_THROWS(calculate_durations({ "2:30pm", "-", "3:00pm", "-", "5:00pm", "-" }));
}

TEST_CASE("Negative durations")
{
    CHECK_THROWS(calculate_durations({ "4:20am", "oh_no_this_will_throw", "4:10am" }));
    CHECK_THROWS(calculate_durations({ "2:30pm", "oh_no_this_will_throw", "2:40am" }));
}

TEST_CASE("Zero length durations")
{
    CHECK_THROWS(calculate_durations({ "9am", "oh_no_this_will_throw", "9am" }));
    CHECK_THROWS(calculate_durations({ "11:00pm", "oh_no_this_will_throw", "11:00pm" }));
}

TEST_CASE("Single duration")
{
    using namespace std::chrono_literals;
    CHECK((DurationMap { { "test", 30min } }) == calculate_durations({ "12pm", "test", "12:30pm" }));
    CHECK((DurationMap { { "test", 2h } }) == calculate_durations({ "2:00pm", "test", "4pm" }));
}

TEST_CASE("Multiple durations")
{
    using namespace std::chrono_literals;
    CHECK((DurationMap { { "one", 30min }, { "two", 1h } })
          == calculate_durations({ "12:00pm", "one", "12:30pm", "two", "1:30pm" }));
    CHECK((DurationMap { { "one", 45min }, { "two", 15min } })
          == calculate_durations({ "4:15pm", "one", "5pm", "two", "5:15pm" }));
    CHECK((DurationMap { { "one", 2h }, { "two", 1h } })
          == calculate_durations({ "8am", "one", "9am", "two", "10:00am", "one", "11:00am" }));
}

TEST_CASE("Formatted output")
{
    using namespace std::chrono_literals;
    CHECK("one     0.5 hours\n"
          "three   1.5 hours\n"
          "two     1.0 hours\n"
          "\nTotal: 3.0 hours (1.0 hours off)\n"
          == format_durations({ { "one", 30min }, { "two", 60min }, { "three", 90min }, { "-", 60min } }));
    CHECK(
        "areallylongactivitythatdominatesthetable   1.1 hours\n"
        "eat                                        2.0 hours\n"
        "sleep                                      8.0 hours\n"
        "\nTotal: 11.1 hours\n"
        == format_durations({ { "eat", 2h }, { "sleep", 8h }, { "areallylongactivitythatdominatesthetable", 66min } }));
    CHECK("a   0.0 hours\n"
          "b   0.1 hours\n"
          "c   0.1 hours\n"
          "d   0.1 hours\n"
          "e   0.2 hours\n"
          "\nTotal: 0.5 hours\n"
          == format_durations({ { "a", 2min }, { "b", 3min }, { "c", 8min }, { "d", 9min }, { "e", 10min } }));
}

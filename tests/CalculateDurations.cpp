#include <timecard/CalculateDurations.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("calculate_duration")
{
    SECTION("Too few to calculate")
    {
        CHECK_THROWS(calculate_durations({}));
        CHECK_THROWS(calculate_durations({ "12:00pm" }));
        CHECK_THROWS(calculate_durations({ "12:00pm", "12:30pm" }));
    }

    SECTION("Extra unparsed arguments")
    {
        CHECK_THROWS(calculate_durations({ "1am", "so_far_so_good", "2:00am", "oh_no_an_extra_label" }));
        CHECK_THROWS(calculate_durations({ "2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "time_to_throw" }));
        CHECK_THROWS(calculate_durations({ "2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "-" }));
        CHECK_THROWS(calculate_durations({ "2:30pm", "-", "3:00pm", "-", "5:00pm", "-" }));
    }

    SECTION("Negative")
    {
        CHECK_THROWS(calculate_durations({ "4:20am", "oh_no_this_will_throw", "4:10am" }));
        CHECK_THROWS(calculate_durations({ "2:30pm", "oh_no_this_will_throw", "2:40am" }));
    }

    SECTION("Zero length")
    {
        CHECK_THROWS(calculate_durations({ "9am", "oh_no_this_will_throw", "9am" }));
        CHECK_THROWS(calculate_durations({ "11:00pm", "oh_no_this_will_throw", "11:00pm" }));
    }

    SECTION("Single")
    {
        using namespace std::chrono_literals;
        CHECK((DurationMap { { "test", 30min } }) == calculate_durations({ "12pm", "test", "12:30pm" }));
        CHECK((DurationMap { { "test", 2h } }) == calculate_durations({ "2:00pm", "test", "4pm" }));
    }

    SECTION("Multiple")
    {
        using namespace std::chrono_literals;
        CHECK((DurationMap { { "one", 30min }, { "two", 1h } })
              == calculate_durations({ "12:00pm", "one", "12:30pm", "two", "1:30pm" }));
        CHECK((DurationMap { { "one", 45min }, { "two", 15min } })
              == calculate_durations({ "4:15pm", "one", "5pm", "two", "5:15pm" }));
        CHECK((DurationMap { { "one", 2h }, { "two", 1h } })
              == calculate_durations({ "8am", "one", "9am", "two", "10:00am", "one", "11:00am" }));
    }
}

#include <CalculateDurations.h>
#include <DurationMap.h>
#include <FormatDurations.h>
#include <ParseTimepoint.h>

#include <gtest/gtest.h>

TEST(Options, GitVersion)
{
#ifndef GIT_VERSION
    FAIL() << "GIT_VERSION symbol not defined";
#endif
    EXPECT_GT(std::string(GIT_VERSION).length(), 0);
    EXPECT_EQ('v', GIT_VERSION[0]);
}

TEST(parse_timepoint, Garbage)
{
    EXPECT_THROW(parse_timepoint(""), std::invalid_argument);
    EXPECT_THROW(parse_timepoint(" "), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("am"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("pm"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("asdf"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("just a plain ole string"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("af13"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("1200tns"), std::invalid_argument);
}

TEST(parse_timepoint, HoursOutOfRange)
{
    EXPECT_THROW(parse_timepoint( "0:00am"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("00:00am"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("13:00am"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("13:00pm"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("20:00pm"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("25:00pm"), std::invalid_argument);
}

TEST(parse_timepoint, MinutesOutOfRange)
{
    EXPECT_THROW(parse_timepoint( "1:60am"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint( "4:90am"), std::invalid_argument);
    EXPECT_THROW(parse_timepoint("12:61pm"), std::invalid_argument);
}

TEST(parse_timepoint, ValidAmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ( 0h +  0min, parse_timepoint("12:00am"));
    EXPECT_EQ( 0h + 30min, parse_timepoint("12:30am"));
    EXPECT_EQ( 2h + 15min, parse_timepoint( "2:15am"));
    EXPECT_EQ( 4h + 20min, parse_timepoint( "4:20am"));
    EXPECT_EQ( 9h + 41min, parse_timepoint( "9:41am"));
    EXPECT_EQ( 7h +  1min, parse_timepoint( "7:01am"));
    EXPECT_EQ(11h +  0min, parse_timepoint("11:00am"));
    EXPECT_EQ(11h + 30min, parse_timepoint("11:30am"));
    EXPECT_EQ(11h + 59min, parse_timepoint("11:59am"));
}

TEST(parse_timepoint, ValidPmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(12h +  0min, parse_timepoint("12:00pm"));
    EXPECT_EQ(12h + 30min, parse_timepoint("12:30pm"));
    EXPECT_EQ(13h +  0min, parse_timepoint( "1:00pm"));
    EXPECT_EQ(14h +  0min, parse_timepoint( "2:00pm"));
    EXPECT_EQ(14h + 30min, parse_timepoint( "2:30pm"));
    EXPECT_EQ(16h +  6min, parse_timepoint( "4:06pm"));
    EXPECT_EQ(22h +  7min, parse_timepoint("10:07pm"));
    EXPECT_EQ(23h + 59min, parse_timepoint("11:59pm"));
}

TEST(parse_timepoint, ShortTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ( 0h, parse_timepoint("12am"));
    EXPECT_EQ( 1h, parse_timepoint( "1am"));
    EXPECT_EQ( 8h, parse_timepoint( "8am"));
    EXPECT_EQ(12h, parse_timepoint("12pm"));
    EXPECT_EQ(13h, parse_timepoint( "1pm"));
    EXPECT_EQ(23h, parse_timepoint("11pm"));
}

TEST(DurationMap, MappedType)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(0h,   DurationMap::mapped_type(0));
    EXPECT_EQ(1h,   DurationMap::mapped_type(1));
    EXPECT_EQ(5h,   DurationMap::mapped_type(5));
    EXPECT_EQ(10h,  DurationMap::mapped_type(10));
    EXPECT_EQ(100h, DurationMap::mapped_type(100));
}

TEST(calculate_durations, TooFewArguments)
{
    EXPECT_THROW(calculate_durations({}), std::invalid_argument);
    EXPECT_THROW(calculate_durations({"12:00pm"}), std::invalid_argument);
    EXPECT_THROW(calculate_durations({"12:00pm", "12:30pm"}), std::invalid_argument);
}

TEST(calculate_durations, ExtraArgument)
{
    EXPECT_THROW(calculate_durations({"1am", "so_far_so_good", "2:00am", "oh_no_an_extra_label"}), std::invalid_argument);
    EXPECT_THROW(calculate_durations({"2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "time_to_throw"}), std::invalid_argument);
    EXPECT_THROW(calculate_durations({"2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "-"}), std::invalid_argument);
    EXPECT_THROW(calculate_durations({"2:30pm", "-", "3:00pm", "-", "5:00pm", "-"}), std::invalid_argument);
}

TEST(calculate_durations, NegativeDuration)
{
    EXPECT_THROW(calculate_durations({"4:20am", "oh_no_this_will_throw", "4:10am"}), std::runtime_error);
    EXPECT_THROW(calculate_durations({"2:30pm", "oh_no_this_will_throw", "2:40am"}), std::runtime_error);
}

TEST(calculate_durations, ZeroDuration)
{
    EXPECT_THROW(calculate_durations({"9am", "oh_no_this_will_throw", "9am"}), std::runtime_error);
    EXPECT_THROW(calculate_durations({"11:00pm", "oh_no_this_will_throw", "11:00pm"}), std::runtime_error);
}

TEST(calculate_durations, SingleDuration)
{
    using namespace std::chrono_literals;
    EXPECT_EQ((DurationMap{{"test", 30min}}), calculate_durations({"12pm", "test", "12:30pm"}));
    EXPECT_EQ((DurationMap{{"test", 2h}}),    calculate_durations({"2:00pm", "test", "4pm"}));
}

TEST(calculate_durations, MultiDurations)
{
    using namespace std::chrono_literals;
    EXPECT_EQ((DurationMap{{"one", 30min}, {"two", 1h}}),    calculate_durations({"12:00pm", "one", "12:30pm", "two", "1:30pm"}));
    EXPECT_EQ((DurationMap{{"one", 45min}, {"two", 15min}}), calculate_durations({"4:15pm", "one", "5pm", "two", "5:15pm"}));
    EXPECT_EQ((DurationMap{{"one", 2h},    {"two", 1h}}),    calculate_durations({"8am", "one", "9am", "two", "10:00am", "one", "11:00am"}));
}

TEST(format_durations, ExactOutput)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(
        "one     0.5 hours\n"
        "three   1.5 hours\n"
        "two     1.0 hours\n"
        "\nTotal: 3.0 hours (1.0 hours off)\n",
        format_durations({{"one", 30min}, {"two", 60min}, {"three", 90min}, {"-", 60min}}));
    EXPECT_EQ(
        "areallylongactivitythatdominatesthetable   1.1 hours\n"
        "eat                                        2.0 hours\n"
        "sleep                                      8.0 hours\n"
        "\nTotal: 11.1 hours\n",
        format_durations({{"eat", 2h}, {"sleep", 8h}, {"areallylongactivitythatdominatesthetable", 66min}}));
    EXPECT_EQ(
        "a   0.0 hours\n"
        "b   0.1 hours\n"
        "c   0.1 hours\n"
        "d   0.1 hours\n"
        "e   0.2 hours\n"
        "\nTotal: 0.5 hours\n",
        format_durations({{"a", 2min}, {"b", 3min}, {"c", 8min}, {"d", 9min}, {"e", 10min}}));
}

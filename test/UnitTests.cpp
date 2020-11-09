#include <CheckFlags.h>
#include <ParseTimepoint.h>
#include <CalculateDurations.h>
#include <PrintDurations.h>
#include <VectorizeArguments.h>

#include <gtest/gtest.h>

TEST(VectorizeArguments, SingleArgument)
{
    constexpr int argc = 2;
    constexpr const char* argv[argc] = {"my_program_name", "some_input"};
    EXPECT_EQ(std::vector<std::string>({"some_input"}), VectorizeArguments(argc, argv));
}

TEST(VectorizeArguments, MultipleArguments)
{
    constexpr int argc = 8;
    constexpr const char* argv[argc] = {"timecard", "830am", "breakfast", "900am", "sleep", "1200pm", "lunch", "100pm"};
    EXPECT_EQ(std::vector<std::string>({"830am", "breakfast", "900am", "sleep", "1200pm", "lunch", "100pm"}), VectorizeArguments(argc, argv));
}

TEST(CheckFlags, ExitSuccess)
{
    const auto exit_success = [](const int exit_code) { return exit_code == EXIT_SUCCESS; };
    EXPECT_EXIT(CheckFlags({"-h"}),        exit_success, "");
    EXPECT_EXIT(CheckFlags({"--help"}),    exit_success, "");
    EXPECT_EXIT(CheckFlags({"-v"}),        exit_success, "");
    EXPECT_EXIT(CheckFlags({"--version"}), exit_success, "");
}

TEST(CheckFlags, Throws)
{
    EXPECT_THROW(CheckFlags({"--h"}),      std::invalid_argument);
    EXPECT_THROW(CheckFlags({"-help"}),    std::invalid_argument);
    EXPECT_THROW(CheckFlags({"--v"}),      std::invalid_argument);
    EXPECT_THROW(CheckFlags({"-version"}), std::invalid_argument);
}

TEST(AmPmOffset, Am)
{
    EXPECT_EQ(0,  AmPmOffset("8:00am"));
    EXPECT_EQ(0,  AmPmOffset("12:00am"));
}

TEST(AmPmOffset, Pm)
{
    EXPECT_EQ(12, AmPmOffset("10:00pm"));
    EXPECT_EQ(12, AmPmOffset("02:00pm"));
}

TEST(AmPmOffset, Garbage)
{
    EXPECT_THROW(AmPmOffset("asdf"),   std::invalid_argument);
    EXPECT_THROW(AmPmOffset("-  _ -"), std::invalid_argument);
}

TEST(ParseTimepoint, Garbage)
{
    EXPECT_THROW(ParseTimepoint(""), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint(" "), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("asdf"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("just a plain ole string"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("af13"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1200tns"), std::invalid_argument);
}

TEST(ParseTimepoint, HoursOutOfRange)
{
    EXPECT_THROW(ParseTimepoint( "0:00am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("00:00am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("13:00am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("13:00pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("20:00pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("25:00pm"), std::invalid_argument);
}

TEST(ParseTimepoint, MinutesOutOfRange)
{
    EXPECT_THROW(ParseTimepoint( "1:60am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint( "4:90am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("12:61pm"), std::invalid_argument);
}

TEST(ParseTimepoint, ValidAmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ( 0h +  0min, ParseTimepoint("12:00am"));
    EXPECT_EQ( 0h + 30min, ParseTimepoint("12:30am"));
    EXPECT_EQ( 2h + 15min, ParseTimepoint( "2:15am"));
    EXPECT_EQ( 4h + 20min, ParseTimepoint( "4:20am"));
    EXPECT_EQ( 9h + 41min, ParseTimepoint( "9:41am"));
    EXPECT_EQ( 7h +  1min, ParseTimepoint( "7:01am"));
    EXPECT_EQ(11h +  0min, ParseTimepoint("11:00am"));
    EXPECT_EQ(11h + 30min, ParseTimepoint("11:30am"));
    EXPECT_EQ(11h + 59min, ParseTimepoint("11:59am"));
}

TEST(ParseTimepoint, ValidPmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(12h +  0min, ParseTimepoint("12:00pm"));
    EXPECT_EQ(12h + 30min, ParseTimepoint("12:30pm"));
    EXPECT_EQ(13h +  0min, ParseTimepoint( "1:00pm"));
    EXPECT_EQ(14h +  0min, ParseTimepoint( "2:00pm"));
    EXPECT_EQ(14h + 30min, ParseTimepoint( "2:30pm"));
    EXPECT_EQ(16h +  6min, ParseTimepoint( "4:06pm"));
    EXPECT_EQ(22h +  7min, ParseTimepoint("10:07pm"));
    EXPECT_EQ(23h + 59min, ParseTimepoint("11:59pm"));
}

TEST(CalculateDurations, Hours)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(0h,   Hours(0));
    EXPECT_EQ(1h,   Hours(1));
    EXPECT_EQ(5h,   Hours(5));
    EXPECT_EQ(10h,  Hours(10));
    EXPECT_EQ(100h, Hours(100));
}

TEST(CalculateDurations, TooFewArguments)
{
    EXPECT_THROW(CalculateDurations({}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"12:00pm"}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"12:00pm", "12:30pm"}), std::invalid_argument);
}

TEST(CalculateDurations, ExtraArgument)
{
    EXPECT_THROW(CalculateDurations({"1:00am", "so_far_so_good", "2:00am", "oh_no_an_extra_label"}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "time_to_throw"}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"2:30pm", "ok", "3:00pm", "still_good", "5:00pm", "-"}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"2:30pm", "-", "3:00pm", "-", "5:00pm", "-"}), std::invalid_argument);
}

TEST(CalculateDurations, NegativeDuration)
{
    EXPECT_THROW(CalculateDurations({"4:20am", "oh_no_this_will_throw", "4:10am"}), std::runtime_error);
    EXPECT_THROW(CalculateDurations({"2:30pm", "oh_no_this_will_throw", "2:40am"}), std::runtime_error);
}

TEST(CalculateDurations, ZeroDuration)
{
    EXPECT_THROW(CalculateDurations({"9:00am", "oh_no_this_will_throw", "9:00am"}), std::runtime_error);
    EXPECT_THROW(CalculateDurations({"11:00pm", "oh_no_this_will_throw", "11:00pm"}), std::runtime_error);
}

TEST(CalculateDurations, SingleDuration)
{
    using namespace std::chrono_literals;
    EXPECT_EQ((DurationMap{{"test", 30min}}), CalculateDurations({"12:00pm", "test", "12:30pm"}));
    EXPECT_EQ((DurationMap{{"test", 2h}}),    CalculateDurations({"2:00pm", "test", "4:00pm"}));
}

TEST(CalculateDurations, MultiDurations)
{
    using namespace std::chrono_literals;
    EXPECT_EQ((DurationMap{{"one", 30min}, {"two", 1h}}),    CalculateDurations({"12:00pm", "one", "12:30pm", "two", "1:30pm"}));
    EXPECT_EQ((DurationMap{{"one", 45min}, {"two", 15min}}), CalculateDurations({"4:15pm", "one", "5:00pm", "two", "5:15pm"}));
    EXPECT_EQ((DurationMap{{"one", 2h},    {"two", 1h}}),    CalculateDurations({"8:00am", "one", "9:00am", "two", "10:00am", "one", "11:00am"}));
}

TEST(PrintDurations, OffTime)
{
    using namespace std::chrono_literals;
    auto durations = CalculateDurations({"12:00pm", "one", "12:30pm", "-", "2:30pm", "three", "4:00pm"});
    EXPECT_EQ(2h, OffTime(durations));
    EXPECT_TRUE(durations.find("-") == durations.end());
}

TEST(PrintDurations, LongestLabel)
{
    EXPECT_EQ(5,  LongestLabel(CalculateDurations({"12:00pm", "one", "12:30pm", "two", "1:30pm", "-", "2:30pm", "three", "4:00pm"})));
    EXPECT_EQ(7,  LongestLabel(CalculateDurations({"12:00pm", "long", "12:30pm", "longer", "1:30pm", "longest", "4:00pm"})));
    EXPECT_EQ(1,  LongestLabel(CalculateDurations({"12:00pm", "a", "12:30pm"})));
    EXPECT_EQ(26, LongestLabel(CalculateDurations({"12:00pm", "abcdefghijklmnopqrstuvwxyz", "12:30pm"})));
}

TEST(PrintDurations, FormatDurations)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(
        "one     0.5 hours\n"
        "three   1.5 hours\n"
        "two     1.0 hours\n"
        "\nTotal: 3.0 hours (1.0 hours off)\n",
        FormatDurations({{"one", 30min}, {"two", 60min}, {"three", 90min}, {"-", 60min}}).str());
    EXPECT_EQ(
        "areallylongactivitythatdominatesthetable   1.1 hours\n"
        "eat                                        2.0 hours\n"
        "sleep                                      8.0 hours\n"
        "\nTotal: 11.1 hours\n",
        FormatDurations({{"eat", 2h}, {"sleep", 8h}, {"areallylongactivitythatdominatesthetable", 66min}}).str());
    EXPECT_EQ(
        "a   0.0 hours\n"
        "b   0.1 hours\n"
        "c   0.1 hours\n"
        "d   0.1 hours\n"
        "e   0.2 hours\n"
        "\nTotal: 0.5 hours\n",
        FormatDurations({{"a", 2min}, {"b", 3min}, {"c", 8min}, {"d", 9min}, {"e", 10min}}).str());
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

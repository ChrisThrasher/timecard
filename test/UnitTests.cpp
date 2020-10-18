#include <CheckFlags.h>
#include <ParseTimepoint.h>
#include <CalculateDurations.h>
#include <PrintDurations.h>

#include <gtest/gtest.h>

TEST(CheckFlags, ExitSuccess)
{
    const auto exit_success = [](const int exit_code) { return exit_code == 0; };
    EXPECT_EXIT(CheckFlags({"-h"}), exit_success, "");
    EXPECT_EXIT(CheckFlags({"--help"}), exit_success, "");
    EXPECT_EXIT(CheckFlags({"-v"}), exit_success, "");
    EXPECT_EXIT(CheckFlags({"--version"}), exit_success, "");
}


TEST(HasSuffix, HasSuffix)
{
    EXPECT_TRUE(HasSuffix("abcd", "abcd"));
    EXPECT_TRUE(HasSuffix("abcd", "bcd"));
    EXPECT_TRUE(HasSuffix("abcd", "cd"));
    EXPECT_TRUE(HasSuffix("abcd", "d"));
}


TEST(HasSuffix, DoesNotHaveSuffix)
{
    EXPECT_FALSE(HasSuffix("dcba", "abcd"));
    EXPECT_FALSE(HasSuffix("dcba", "bcd"));
    EXPECT_FALSE(HasSuffix("dcba", "cd"));
    EXPECT_FALSE(HasSuffix("dcba", "d"));
}


TEST(HasSuffix, RealTimes)
{
    EXPECT_TRUE(HasSuffix("1000pm", "pm"));
    EXPECT_TRUE(HasSuffix("800am", "am"));
    EXPECT_FALSE(HasSuffix("0200pm", "am"));
    EXPECT_FALSE(HasSuffix("1200am", "pm"));
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


TEST(ParseTimepoint, NearlyGarbage)
{
    EXPECT_THROW(ParseTimepoint("0am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("10"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("20"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("99"), std::invalid_argument);
}


TEST(ParseTimepoint, TwentyFourHourTimes)
{
    EXPECT_THROW(ParseTimepoint("000"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("0000"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("100"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("0100"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1000"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1200"), std::invalid_argument);
}


TEST(ParseTimepoint, NegativeTimes)
{
    EXPECT_THROW(ParseTimepoint("-100am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("-0100am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("-1300am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("-1300pm"), std::invalid_argument);
}


TEST(ParseTimepoint, Invalid12HourTimes)
{
    // Hours exceed [1, 12]
    EXPECT_THROW(ParseTimepoint("000am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("0000am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1300am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1300pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("2000pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("2500pm"), std::invalid_argument);

    // Minutes exceed [0, 59]
    EXPECT_THROW(ParseTimepoint("160am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("490am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1261pm"), std::invalid_argument);
}


TEST(ParseTimepoint, ValidAmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ( 0h +  0min, ParseTimepoint("1200am"));
    EXPECT_EQ( 0h + 30min, ParseTimepoint("1230am"));
    EXPECT_EQ( 2h + 15min, ParseTimepoint("215am"));
    EXPECT_EQ( 4h + 20min, ParseTimepoint("0420am"));
    EXPECT_EQ( 9h + 41min, ParseTimepoint("0941am"));
    EXPECT_EQ( 7h +  1min, ParseTimepoint("0701am"));
    EXPECT_EQ(11h +  0min, ParseTimepoint("1100am"));
    EXPECT_EQ(11h + 30min, ParseTimepoint("1130am"));
    EXPECT_EQ(11h + 59min, ParseTimepoint("1159am"));
}


TEST(ParseTimepoint, ValidPmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(12h +  0min, ParseTimepoint("1200pm"));
    EXPECT_EQ(12h + 30min, ParseTimepoint("1230pm"));
    EXPECT_EQ(13h +  0min, ParseTimepoint("0100pm"));
    EXPECT_EQ(14h +  0min, ParseTimepoint("200pm"));
    EXPECT_EQ(14h + 30min, ParseTimepoint("230pm"));
    EXPECT_EQ(16h +  6min, ParseTimepoint("0406pm"));
    EXPECT_EQ(22h +  7min, ParseTimepoint("1007pm"));
    EXPECT_EQ(23h + 59min, ParseTimepoint("1159pm"));
}


TEST(CalculateDurations, InvalidArgument)
{
    EXPECT_THROW(CalculateDurations({}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"1200pm"}), std::invalid_argument);
    EXPECT_THROW(CalculateDurations({"1200pm", "1230pm"}), std::invalid_argument);
}

TEST(CalculateDurations, NegativeDuration)
{
    EXPECT_THROW(CalculateDurations({"420am", "oh_no_this_will_throw", "410am"}), std::runtime_error);
    EXPECT_THROW(CalculateDurations({"230pm", "oh_no_this_will_throw", "240am"}), std::runtime_error);
}


TEST(CalculateDurations, ZeroDuration)
{
    EXPECT_THROW(CalculateDurations({"900am", "oh_no_this_will_throw", "900am"}), std::runtime_error);
    EXPECT_THROW(CalculateDurations({"1100pm", "oh_no_this_will_throw", "1100pm"}), std::runtime_error);
}


TEST(CalculateDurations, OneDuration)
{
    using namespace std::chrono_literals;
    EXPECT_EQ((DurationMap{{"test", 30min}}), CalculateDurations({"1200pm", "test", "1230pm"}));
    EXPECT_EQ((DurationMap{{"test", 2h}}), CalculateDurations({"200pm", "test", "400pm"}));
}


TEST(CalculateDurations, MultiDurations)
{
    using namespace std::chrono_literals;
    EXPECT_EQ((DurationMap{{"one", 30min}, {"two", 1h}}), CalculateDurations({"1200pm", "one", "1230pm", "two", "130pm"}));
    EXPECT_EQ((DurationMap{{"one", 45min}, {"two", 15min}}), CalculateDurations({"415pm", "one", "500pm", "two", "515pm"}));
    EXPECT_EQ((DurationMap{{"one", 2h}, {"two", 1h}}), CalculateDurations({"800am", "one", "900am", "two", "1000am", "one", "1100am"}));
}


TEST(PrintDurations, LongestLabel)
{
    const auto durations = CalculateDurations({"1200pm", "one", "1230pm", "two", "130pm", "-", "230pm", "three", "400pm"});
    EXPECT_EQ(5, LongestLabel(durations));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

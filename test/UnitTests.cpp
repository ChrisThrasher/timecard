#include <ParseTimepoint.h>

#include <gtest/gtest.h>

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
    EXPECT_EQ(0h + 0min, ParseTimepoint("000am"));
    EXPECT_EQ(0h + 0min, ParseTimepoint("0000am"));
    EXPECT_EQ(0h + 0min, ParseTimepoint("1200am"));
    EXPECT_EQ(0h + 30min, ParseTimepoint("030am"));
    EXPECT_EQ(0h + 30min, ParseTimepoint("0030am"));
    EXPECT_EQ(0h + 30min, ParseTimepoint("1230am"));
    EXPECT_EQ(11h + 0min, ParseTimepoint("1100am"));
    EXPECT_EQ(11h + 30min, ParseTimepoint("1130am"));
}


TEST(ParseTimepoint, ValidPmTimes)
{
    using namespace std::chrono_literals;
    EXPECT_EQ(12h + 0min, ParseTimepoint("1200pm"));
    EXPECT_EQ(12h + 30min, ParseTimepoint("1230pm"));
    EXPECT_EQ(14h + 0min, ParseTimepoint("200pm"));
    EXPECT_EQ(14h + 30min, ParseTimepoint("230pm"));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

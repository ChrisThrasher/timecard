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
    EXPECT_THROW(ParseTimepoint("0am"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1pm"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("10"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("20"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("99"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("af13"), std::invalid_argument);
    EXPECT_THROW(ParseTimepoint("1200tns"), std::invalid_argument);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

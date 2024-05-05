#include <catch2/catch_test_macros.hpp>

import timecard;

TEST_CASE("format_durations")
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

#include "CalculateDurations.h"
#include "FormatDurations.h"

#include <argon/Parser.h>

#include <iostream>

static constexpr auto help = R"(
Time formatting can follow one of two patterns depending on the time it
represents. 8:00 a.m. can be formatted as "8:00am" or "8am". 12:30 p.m. is
formatted only as "12:30pm". "now" is interpreted as the current time.

Any activities named "-" will be ignored. This activity's durations are
reported as "off time" should they exist.)";

int main(int argc, char* argv[])
try {
    argon::Parser parser(argc, argv);
    parser.add_option("h,help", "Show this help text", argon::USAGE, help);
    parser.add_option("v,version", "Print program version", argon::PRINT, TIMECARD_VERSION);
    parser.add_position("args...", "Alternating times and activities");
    parser.parse();

    auto args = parser.args();
    auto durations = calculate_durations(args);
    std::cout << format_durations(durations);
} catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}

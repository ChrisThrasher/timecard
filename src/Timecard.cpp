#include <CalculateDurations.h>
#include <FormatDurations.h>

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
    parser.AddOption("h,help", "Show this help text", argon::USAGE, help);
    parser.AddOption("v,version", "Print program version", argon::PRINT, GIT_VERSION);
    parser.AddPosition("args...", "Alternating times and activities");
    parser.Parse();

    std::cout << format_durations(calculate_durations(parser.Args()));
} catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}

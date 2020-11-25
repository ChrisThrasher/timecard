#include <CalculateDurations.h>
#include <FormatDurations.h>

#include <Options/Parser.h>

static constexpr auto help = R"(Usage
  timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> <timeN>
  timecard [options]

Time formatting can follow one of two patterns depending on the time it
represents. 8:00 a.m. can be formatted as "8:00am" or "8am". 12:30 p.m. is
formatted only as "12:30pm".

Any activities named "-" will be ignored. This activity's durations are
reported as "off time" should they exist.)";

int main(int argc, char* argv[])
try
{
    opts::Parser parser(argc, argv, help);
    parser.Add("v,version", "Print program version", opts::Exit(GIT_VERSION));
    parser.Parse();

    std::cout << FormatDurations(CalculateDurations(parser.Args()));
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}

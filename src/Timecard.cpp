#include <CalculateDurations.h>
#include <FormatDurations.h>
#include <VectorizeArguments.h>

#include <Options.h>

static constexpr auto help_text = R"(Usage
  timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> <timeN>
  timecard [options]

Time formatting can follow one of two patterns depending on the time it
represents. 8:00 a.m. can be formatted as "8:00am" or "8am". 12:30 p.m. is
formatted only as "12:30pm".

Any activities named "-" will be ignored. This activity's durations are
reported as "off time" should they exist.

Options
  -h, --help        Show this help text
  -v, --version     Print program version
)";

int main(int argc, char* argv[])
try
{
    Options options(argc, argv);
    options.ExitItem({"-h", "--help"}, help_text);
    options.ExitItem({"-v", "--version"}, GIT_VERSION);

    std::cout << FormatDurations(CalculateDurations(VectorizeArguments(argc, argv)));
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}

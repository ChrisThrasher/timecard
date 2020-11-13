#pragma once

#include <iostream>
#include <string>
#include <vector>

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

void CheckFlags(const std::vector<std::string>& args)
{
    if (args.empty())
        return;

    const auto flag = args[0];
    if (flag == "-h" or flag == "--help")
    {
        std::cout << help_text;
        std::exit(EXIT_SUCCESS);
    }
    if (flag == "-v" or flag == "--version")
    {
        std::cout << "Timecard version " << GIT_VERSION << '\n';
        std::exit(EXIT_SUCCESS);
    }
    if (flag[0] == '-')
    {
        throw std::invalid_argument("Unrecognized option: " + flag + '\n' + help_text);
    }
}

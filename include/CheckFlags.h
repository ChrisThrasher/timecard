#pragma once

#include <iostream>
#include <string>
#include <vector>

static constexpr auto help_text = R"(Usage
  timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> <timeN>
  timecard [options]

Times must follow the following regex:

  ([1][0-2]|[1-9]):[0-5][0-9](a|p)m

For examle, 8:00 a.m. is represented as "8:00am". 12:30 p.m. is represented as
"12:30pm".

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

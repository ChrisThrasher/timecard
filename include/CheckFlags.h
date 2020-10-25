#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static constexpr auto help_text = R"(Usage
  timecard [options]
  timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> <timeN>

Times are formatted as 3 or 4 digits followed by either an 'am' or 'pm' suffix.
Any activities named "-" will be ignored. For example, 8:00am is represented as
"800am". 12:30pm is represented as "1230pm".

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
        throw std::invalid_argument("Unrecognized flag: " + flag + '\n' + help_text);
    }
}

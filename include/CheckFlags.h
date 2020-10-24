#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static constexpr auto help_text = R"(Usage: timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> <timeN>

Times are formatted as 3 or 4 digits followed by either an 'am' or 'pm' suffix.
Any activities named "-" will be ignored.
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
        throw std::invalid_argument("Unrecognized flag: " + flag);
    }
}

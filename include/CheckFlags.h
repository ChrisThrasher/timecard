#pragma once

#include <iostream>
#include <string>
#include <vector>

void CheckFlags(const std::vector<std::string>& args)
{
    if (args.size() > 1 and (args[1] == "-h" or args[1] == "--help"))
    {
        std::cout << "Usage: timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> "
                     "<timeN>\n\n";
        std::cout << "Times are formatted as 3 or 4 digits followed by either an 'am' or 'pm' suffix.\n";
        std::exit(0);
    }

    if (args.size() > 1 and (args[1] == "-v" or args[1] == "--version"))
    {
        std::cout << "Timecard version " << GIT_VERSION << '\n';
        std::exit(0);
    }
}

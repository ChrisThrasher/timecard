#include <ParseTimepoint.h>
#include <VectorizeArguments.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>

int main(int argc, char* argv[])
try
{
    const auto args = VectorizeArguments(argc, argv);
    if (args.size() > 1 and (args[1] == "-h" or args[1] == "--help"))
    {
        std::cout << "Usage: timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> "
                     "<timeN>\n\n";
        std::cout
            << "Times are formatted as 3 or 4 digits followed by either an 'am' or 'pm' suffix.\n";
        return 0;
    }

    if (args.size() > 1 and (args[1] == "-v" or args[1] == "--version"))
    {
        std::cout << "Timecard version " << GIT_VERSION <<'\n';
        return 0;
    }

    std::map<std::string, std::chrono::duration<double, std::ratio<3600>>> durations;
    for (size_t i = 2; i + 1 < args.size(); i += 2)
    {
        const auto key = args[i];
        const auto duration = ParseTimepoint(args[i + 1]) - ParseTimepoint(args[i - 1]);
        if (duration < std::chrono::minutes(0))
        {
            throw std::runtime_error("Duration from " + args[i - 1] + " to " + args[i + 1] +
                                     " is negative.");
        }
        if (durations.find(key) == durations.end())
            durations[key] = duration;
        else
            durations.at(key) += duration;
    }

    std::cout << std::fixed << std::setprecision(1);
    for (const auto& duration : durations)
    {
        std::cout << duration.first << ": " << duration.second.count() << " hours\n";
    }
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << '\n';
    std::cerr << "Failed to parse arguments. Exiting.\n" << std::flush;
    return 1;
}
catch (...)
{
    std::cerr << "Failed to parse arguments. Exiting.\n" << std::flush;
    return 1;
}

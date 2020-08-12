#include <ParseTimepoint.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>

int main(int argc, char* argv[])
try
{
    if (argc > 1 and (std::string(argv[1]) == "-h" or std::string(argv[1]) == "--help"))
    {
        std::cout << "Usage: timecard <time1> <activity1> <time2> <activity2> <time3> <activityN> "
                     "<timeN>\n";
        return 0;
    }

    std::map<std::string, std::chrono::duration<double, std::ratio<3600>>> durations;
    for (int i = 2; i + 1 < argc; i += 2)
    {
        const auto key = argv[i];
        const auto duration = ParseTimepoint(argv[i + 1]) - ParseTimepoint(argv[i - 1]);
        if (duration < std::chrono::minutes(0))
        {
            throw std::runtime_error(std::string("Duration from ") + argv[i - 1] + " to " +
                                     argv[i + 1] + " is negative.");
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

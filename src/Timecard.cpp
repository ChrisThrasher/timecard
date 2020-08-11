#include <chrono>
#include <iostream>
#include <map>

auto ParseTimepoint(const std::string& timepoint)
{
    try
    {
        if (timepoint.size() > 4 or timepoint.size() < 3)
        {
            throw std::invalid_argument("Timepoint string must be 3 or 4 characters long.");
        }

        const auto time = std::stoi(timepoint);
        const auto minutes = time % 100;
        const auto hours = (time - minutes) / 100;

        if (minutes >= 60)
        {
            throw std::invalid_argument("Minutes are greater than 59.");
        }
        if (hours >= 24)
        {
            throw std::invalid_argument("Hours are greater than 23.");
        }

        return std::chrono::hours(hours) + std::chrono::minutes(minutes);
    }
    catch (...)
    {
        std::cerr << "Could not parse \"" << timepoint << "\" as a time." << std::endl;
        throw;
    }
}

int main(int argc, char* argv[])
try
{
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

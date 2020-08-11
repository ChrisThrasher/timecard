#include <chrono>
#include <iostream>
#include <map>

auto ParseTimepoint(std::string timepoint)
{
    const auto has_suffix = [timepoint](const std::string& suffix) {
        return timepoint.size() >= suffix.size() and
               timepoint.compare(timepoint.size() - suffix.size(), suffix.size(), suffix) == 0;
    };

    try
    {
        auto am_pm_offset = 0;
        if (has_suffix("am"))
        {
            am_pm_offset = 0;
            timepoint = timepoint.substr(0, timepoint.size() - 2);
        }
        if (has_suffix("pm"))
        {
            am_pm_offset = 12;
            timepoint = timepoint.substr(0, timepoint.size() - 2);
        }

        if (timepoint.size() > 4 or timepoint.size() < 3)
        {
            throw std::invalid_argument(
                "Timepoint string must contain 3 or 4 consecutive numbers.");
        }

        const auto time = std::stoi(timepoint);
        const auto minutes = time % 100;
        const auto hours_unadjusted = (time - minutes) / 100;
        const auto hours =
            hours_unadjusted != 12 ? hours_unadjusted + am_pm_offset : hours_unadjusted;

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

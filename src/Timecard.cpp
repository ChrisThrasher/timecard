#include <chrono>
#include <iostream>
#include <map>

auto ParseTimepoint(const std::string& timepoint)
{
    if (timepoint.size() > 4 or
        timepoint.size() < 3)
    {
        throw std::invalid_argument("Timepoint string must be 3 or 4 characters long");
    }

    try
    {
        const auto time = std::stoi(timepoint);
        const auto minutes = std::chrono::minutes(time % 100);
        const auto hours = std::chrono::hours((time - (time % 100)) / 100);

        return hours + minutes;
    }
    catch (...)
    {
        std::cerr << "Could not parse \"" << timepoint << '"' << std::endl;
        throw;
    }
}

int main(int argc, char* argv[]) try
{
    std::map<std::string, std::chrono::minutes> durations;
    for (int i = 2; i + 1 < argc; i += 2)
    {
        const auto key = argv[i];
        const auto duration = ParseTimepoint(argv[i + 1]) - ParseTimepoint(argv[i - 1]);
        if (durations.find(key) == durations.end())
            durations[key] = duration;
        else
            durations.at(key) += duration;
    }

    for (const auto duration : durations)
    {
        std::cout << duration.first << ": " << duration.second.count() << " minutes\n";
    }
}
catch (...)
{
    std::cerr << "Failed to parse arguments. Exiting.\n" << std::flush;
    return 1;
}

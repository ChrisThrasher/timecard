#include <CheckFlags.h>
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
    CheckFlags(args);

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

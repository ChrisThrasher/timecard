#pragma once

#include <ParseTimepoint.h>

#include <chrono>
#include <map>
#include <string>
#include <vector>

auto CalculateDurations(const std::vector<std::string>& args)
    -> std::map<std::string, std::chrono::duration<double, std::ratio<3600>>>
{
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

    return durations;
}

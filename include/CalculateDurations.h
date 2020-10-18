#pragma once

#include <ParseTimepoint.h>

#include <chrono>
#include <map>
#include <string>
#include <vector>

using DurationMap = std::map<std::string, std::chrono::duration<double, std::ratio<3600>>>;

auto CalculateDurations(const std::vector<std::string>& args) -> DurationMap
{
    DurationMap durations;

    for (size_t i = 1; i + 1 < args.size(); i += 2)
    {
        const auto key = args[i];
        const auto duration = ParseTimepoint(args[i + 1]) - ParseTimepoint(args[i - 1]);
        if (duration <= std::chrono::minutes(0))
        {
            throw std::runtime_error("Duration from " + args[i - 1] + " to " + args[i + 1] + " is not positive.");
        }
        if (durations.find(key) == durations.end())
            durations[key] = duration;
        else
            durations.at(key) += duration;
    }

    return durations;
}

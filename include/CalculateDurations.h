#pragma once

#include <ParseTimepoint.h>

#include <chrono>
#include <map>
#include <string>
#include <vector>

using DurationType = std::chrono::duration<double, std::ratio<3600>>;
using DurationMap = std::map<std::string, DurationType>;

auto CalculateDurations(const std::vector<std::string>& args) -> DurationMap
{
    if (args.size() < 3)
        throw std::invalid_argument("Received " + std::to_string(args.size()) + " arguments. Expected 3 or more.");
    if (args.size() % 2 == 0)
        std::cout << "Received even number of arguments. Ignoring \"" << args.back() << "\".\n";

    DurationMap durations;

    for (size_t i = 1; i + 1 < args.size(); i += 2)
    {
        const auto duration = -ParseTimepoint(args[i - 1]) + ParseTimepoint(args[i + 1]);
        if (duration <= std::chrono::minutes(0))
            throw std::runtime_error("Duration from " + args[i - 1] + " to " + args[i + 1] + " is not positive.");

        const auto key = args[i];
        durations.insert({key, DurationType(0)});
        durations.at(key) += duration;
    }

    return durations;
}

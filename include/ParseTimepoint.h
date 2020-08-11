#pragma once

#include <chrono>
#include <iostream>
#include <string>

auto ParseTimepoint(std::string timepoint)
try
{
    const auto has_suffix = [timepoint](const std::string& suffix) {
        return timepoint.size() >= suffix.size() and
               timepoint.compare(timepoint.size() - suffix.size(), suffix.size(), suffix) == 0;
    };

    auto am_pm_offset = 0;
    if (has_suffix("am"))
    {
        am_pm_offset = 0;
        timepoint = timepoint.substr(0, timepoint.size() - 2);
    }
    else if (has_suffix("pm"))
    {
        am_pm_offset = 12;
        timepoint = timepoint.substr(0, timepoint.size() - 2);
    }
    else
    {
        throw std::invalid_argument("Found no am or pm prefix.");
    }

    if (timepoint.size() > 4 or timepoint.size() < 3)
    {
        throw std::invalid_argument("Timepoint string must contain 3 or 4 consecutive numbers.");
    }

    const auto time = std::stoi(timepoint);
    const auto minutes = time % 100;
    if (minutes >= 60 or minutes < 0)
    {
        throw std::invalid_argument("Minutes not in range [0, 59].");
    }

    const auto raw_hours = (time - minutes) / 100;
    if (raw_hours > 12 or raw_hours <= 0)
    {
        throw std::invalid_argument("Hours not in range [1, 12].");
    }

    const auto hours = raw_hours % 12 + am_pm_offset;

    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}
catch (...)
{
    std::cerr << "Could not parse \"" << timepoint << "\" as a time." << std::endl;
    throw;
}

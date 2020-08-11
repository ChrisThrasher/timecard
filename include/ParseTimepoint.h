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
    if (has_suffix("pm"))
    {
        am_pm_offset = 12;
        timepoint = timepoint.substr(0, timepoint.size() - 2);
    }

    if (timepoint.size() > 4 or timepoint.size() < 3)
    {
        throw std::invalid_argument("Timepoint string must contain 3 or 4 consecutive numbers.");
    }

    const auto time = std::stoi(timepoint);
    const auto minutes = time % 100;
    const auto hours = ((time - minutes) / 100) % 12 + am_pm_offset;

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

#pragma once

#include <chrono>
#include <iostream>
#include <string>

bool HasSuffix(const std::string& timepoint, const std::string& suffix)
{
    return timepoint.size() >= suffix.size() and
           timepoint.compare(timepoint.size() - suffix.size(), suffix.size(), suffix) == 0;
}

auto ParseTimepoint(const std::string& timepoint)
try
{

    auto am_pm_offset = 0;
    auto modified_timepoint = timepoint;
    if (HasSuffix(timepoint, "am"))
    {
        am_pm_offset = 0;
        modified_timepoint = timepoint.substr(0, timepoint.size() - 2);
    }
    else if (HasSuffix(timepoint, "pm"))
    {
        am_pm_offset = 12;
        modified_timepoint = timepoint.substr(0, timepoint.size() - 2);
    }
    else
    {
        throw std::invalid_argument("Found no am or pm prefix.");
    }

    if (modified_timepoint.size() > 4 or modified_timepoint.size() < 3)
    {
        throw std::invalid_argument("Timepoint string must contain 3 or 4 consecutive numbers.");
    }

    const auto time = std::stoi(modified_timepoint);
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

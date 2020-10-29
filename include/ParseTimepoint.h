#pragma once

#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

bool HasSuffix(const std::string& timepoint, const std::string& suffix)
{
    return timepoint.size() >= suffix.size() and
           timepoint.compare(timepoint.size() - suffix.size(), suffix.size(), suffix) == 0;
}

auto ParseTimepoint(const std::string& timepoint)
try
{
    const auto is_am = HasSuffix(timepoint, "am");
    const auto is_pm = HasSuffix(timepoint, "pm");
    if (not is_am and not is_pm)
        throw std::invalid_argument("No am or pm prefix.");

    const auto modified_timepoint = timepoint.substr(0, timepoint.size() - 2);
    if (modified_timepoint.size() > 4 or modified_timepoint.size() < 3)
        throw std::invalid_argument("Must contain 3 or 4 consecutive numbers.");

    const auto time = std::stoi(modified_timepoint);
    const auto minutes = time % 100;
    if (minutes >= 60 or minutes < 0)
        throw std::invalid_argument("Minutes not in range [0, 59].");

    const auto raw_hours = (time - minutes) / 100;
    if (raw_hours > 12 or raw_hours <= 0)
        throw std::invalid_argument("Hours not in range [1, 12].");

    const auto hours = raw_hours % 12 + (is_pm ? 12 : 0);
    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}
catch (const std::exception& ex)
{
    throw std::invalid_argument("Failed to parse \"" + timepoint + "\". " + ex.what());
}

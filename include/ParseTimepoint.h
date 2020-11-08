#pragma once

#include <chrono>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

bool HasSuffix(const std::string& timepoint, const std::string& suffix)
{
    return std::regex_match(timepoint, std::regex(".*" + suffix));
}

auto ParseTimepoint(const std::string& timepoint)
try
{
    const auto is_am = HasSuffix(timepoint, "am");
    const auto is_pm = HasSuffix(timepoint, "pm");
    if (not is_am and not is_pm)
        throw std::invalid_argument("No am or pm suffix.");

    const auto time = std::stoi(timepoint.substr(0, timepoint.size() - 2));
    const auto minutes = time % 100;
    if (minutes > 59 or minutes < 0)
        throw std::invalid_argument("Minutes (" + std::to_string(minutes) + ") not in range [0, 59].");

    const auto raw_hours = (time - minutes) / 100;
    if (raw_hours > 12 or raw_hours < 1)
        throw std::invalid_argument("Hours (" + std::to_string(raw_hours) + ") not in range [1, 12].");

    const auto hours = raw_hours % 12 + (is_pm ? 12 : 0);
    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}
catch (const std::exception& ex)
{
    throw std::invalid_argument("Failed to parse \"" + timepoint + "\". " + ex.what());
}

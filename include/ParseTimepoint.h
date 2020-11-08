#pragma once

#include <chrono>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

int AmPmOffset(const std::string& timepoint)
{
    if (std::regex_match(timepoint, std::regex(".*pm")))
        return 12;
    if (std::regex_match(timepoint, std::regex(".*am")))
        return 0;
    throw std::invalid_argument("Found no am/pm suffix");
}

auto ParseTimepoint(const std::string& timepoint)
try
{
    constexpr auto timepoint_regex = "\\d{3,4}(a|p)m";
    if (not std::regex_match(timepoint, std::regex(timepoint_regex)))
        throw std::invalid_argument(std::string("Did not match regex: ") + timepoint_regex);

    const auto time = std::stoi(timepoint.substr(0, timepoint.size() - 2));
    const auto minutes = time % 100;
    if (minutes > 59 or minutes < 0)
        throw std::invalid_argument("Minutes (" + std::to_string(minutes) + ") not in range [0, 59]");

    const auto raw_hours = (time - minutes) / 100;
    if (raw_hours > 12 or raw_hours < 1)
        throw std::invalid_argument("Hours (" + std::to_string(raw_hours) + ") not in range [1, 12]");

    const auto hours = raw_hours % 12 + AmPmOffset(timepoint);
    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}
catch (const std::exception& ex)
{
    throw std::invalid_argument("Failed to parse \"" + timepoint + "\": " + ex.what());
}

#pragma once

#include <chrono>
#include <regex>
#include <sstream>
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
{
    if (not std::regex_match(timepoint, std::regex("([1][0-2]|[1-9]):[0-5][0-9](a|p)m")))
        throw std::invalid_argument("Failed to parse \"" + timepoint + "\"");

    const auto hours = std::stoi(timepoint.substr(0, timepoint.size() - 5)) % 12 + AmPmOffset(timepoint);
    const auto minutes = std::stoi(timepoint.substr(timepoint.size() - 4, 2));

    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}

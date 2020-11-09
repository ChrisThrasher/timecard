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
{
    if (not std::regex_match(timepoint, std::regex("([1][0-2]|[1-9])[0-5][0-9](a|p)m")))
        throw std::invalid_argument("Failed to parse \"" + timepoint + "\"");

    const auto time = std::stoi(timepoint.substr(0, timepoint.size() - 2));

    const auto minutes = time % 100;

    const auto raw_hours = (time - minutes) / 100;
    const auto hours = raw_hours % 12 + AmPmOffset(timepoint);

    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}

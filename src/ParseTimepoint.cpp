#include <ParseTimepoint.h>

#include <chrono>
#include <regex>
#include <string>

static int am_pm_offset(const std::string& timepoint)
{
    if (std::regex_match(timepoint, std::regex(".*pm")))
        return 12;
    if (std::regex_match(timepoint, std::regex(".*am")))
        return 0;
    throw std::invalid_argument("Found no am/pm suffix");
}

static auto parse_long_timepoint(const std::string& timepoint)
{
    const auto hours = std::stoi(timepoint.substr(0, timepoint.size() - 5)) % 12 + am_pm_offset(timepoint);
    const auto minutes = std::stoi(timepoint.substr(timepoint.size() - 4, 2));

    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}

static auto parse_short_timepoint(const std::string& timepoint)
{
    const auto hours = std::stoi(timepoint.substr(0, timepoint.size() - 2)) % 12 + am_pm_offset(timepoint);

    return std::chrono::hours(hours);
}

static auto current_time()
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const tm time = *localtime(&now);

    return std::chrono::hours(time.tm_hour) + std::chrono::minutes(time.tm_min);
}

auto parse_timepoint(const std::string& timepoint) -> std::chrono::minutes
{
    if (std::regex_match(timepoint, std::regex("([1][0-2]|[1-9]):[0-5][0-9](a|p)m")))
        return parse_long_timepoint(timepoint);
    if (std::regex_match(timepoint, std::regex("([1][0-2]|[1-9])(a|p)m")))
        return parse_short_timepoint(timepoint);
    if (std::regex_match(timepoint, std::regex("now")))
        return current_time();

    throw std::invalid_argument("Failed to parse \"" + timepoint + "\"");
}

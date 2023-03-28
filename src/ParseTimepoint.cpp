#include <timecard/ParseTimepoint.hpp>

#include <regex>
#include <string>

namespace {
[[nodiscard]] auto am_pm_offset(std::string_view timepoint)
{
    if (std::regex_match(std::string(timepoint), std::regex(".*pm")))
        return 12;
    if (std::regex_match(std::string(timepoint), std::regex(".*am")))
        return 0;
    throw std::invalid_argument("Found no am/pm suffix");
}

[[nodiscard]] auto parse_long_timepoint(std::string_view timepoint)
{
    const auto hours = std::stoi(std::string(timepoint.substr(0, timepoint.size() - 5))) % 12 + am_pm_offset(timepoint);
    const auto minutes = std::stoi(std::string(timepoint.substr(timepoint.size() - 4, 2)));

    return std::chrono::hours(hours) + std::chrono::minutes(minutes);
}

[[nodiscard]] auto parse_short_timepoint(std::string_view timepoint)
{
    const auto hours = std::stoi(std::string(timepoint.substr(0, timepoint.size() - 2))) % 12 + am_pm_offset(timepoint);

    return std::chrono::hours(hours);
}

[[nodiscard]] auto current_time()
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto time = *localtime(&now);

    return std::chrono::hours(time.tm_hour) + std::chrono::minutes(time.tm_min);
}
}

auto parse_timepoint(std::string_view timepoint) -> std::chrono::minutes
{
    if (std::regex_match(std::string(timepoint), std::regex("([1][0-2]|[1-9]):[0-5][0-9](a|p)m")))
        return parse_long_timepoint(timepoint);
    if (std::regex_match(std::string(timepoint), std::regex("([1][0-2]|[1-9])(a|p)m")))
        return parse_short_timepoint(timepoint);
    if (std::regex_match(std::string(timepoint), std::regex("now")))
        return current_time();

    throw std::invalid_argument("Failed to parse \"" + std::string(timepoint) + "\"");
}

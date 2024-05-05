module;

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <map>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

export module timecard;

export using DurationMap = std::map<std::string_view, std::chrono::minutes>;

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

export [[nodiscard]] auto parse_timepoint(std::string_view timepoint) -> std::chrono::minutes
{
    if (std::regex_match(std::string(timepoint), std::regex("([1][0-2]|[1-9]):[0-5][0-9](a|p)m")))
        return parse_long_timepoint(timepoint);
    if (std::regex_match(std::string(timepoint), std::regex("([1][0-2]|[1-9])(a|p)m")))
        return parse_short_timepoint(timepoint);
    if (std::regex_match(std::string(timepoint), std::regex("now")))
        return current_time();

    throw std::invalid_argument("Failed to parse \"" + std::string(timepoint) + "\"");
}

[[nodiscard]] auto longest_label(const DurationMap& durations)
{
    return static_cast<int>(std::ranges::max_element(durations, [](const auto& lhs, const auto& rhs) {
                                return lhs.first.length() < rhs.first.length();
                            })->first.length());
}

[[nodiscard]] auto as_hours(const std::chrono::minutes& minutes)
{
    return std::chrono::duration<double, std::ratio<3600>>(minutes).count();
}

export [[nodiscard]] auto calculate_durations(const std::vector<std::string_view>& args) -> DurationMap
{
    if (args.size() < 3)
        throw std::invalid_argument("Received " + std::to_string(args.size()) + " arguments. Expected 3 or more.");
    if (args.size() % 2 == 0)
        throw std::invalid_argument("Received even number of arguments. Ignoring \"" + std::string(args.back())
                                    + "\".");

    DurationMap durations;

    for (size_t i = 1; i + 1 < args.size(); i += 2) {
        const auto duration = -parse_timepoint(args[i - 1]) + parse_timepoint(args[i + 1]);
        if (duration <= std::chrono::minutes(0))
            throw std::runtime_error("Duration from " + std::string(args[i - 1]) + " to " + std::string(args[i + 1])
                                     + " is not positive.");

        const auto& key = args[i];
        durations.insert({ key, {} });
        durations.at(key) += duration;
    }

    return durations;
}

export [[nodiscard]] auto format_durations(DurationMap durations) -> std::string
{
    using namespace std::chrono_literals;
    const auto off_time = durations.find("-") != durations.end() ? durations["-"] : 0min;
    durations.erase("-");

    const auto label_width = longest_label(durations) + 2;

    std::stringstream out;
    out << std::fixed << std::setprecision(1) << std::setfill(' ');

    auto total = 0min;
    for (const auto& duration : durations) {
        out << std::left << std::setw(label_width) << duration.first;
        out << std::right << std::setw(4) << as_hours(duration.second) << " hours\n";
        total += duration.second;
    }

    out << "\nTotal: " << as_hours(total) << " hours";
    if (off_time > 0min)
        out << " (" << as_hours(off_time) << " hours off)";
    out << '\n';

    return out.str();
}

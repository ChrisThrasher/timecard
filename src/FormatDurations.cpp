#include <timecard/FormatDurations.hpp>

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace {
[[nodiscard]] auto longest_label(const DurationMap& durations)
{
    return static_cast<int>(
        std::max_element(std::begin(durations), std::end(durations), [](const auto& lhs, const auto& rhs) {
            return lhs.first.length() < rhs.first.length();
        })->first.length());
}

[[nodiscard]] auto as_hours(const std::chrono::minutes& minutes)
{
    return std::chrono::duration<double, std::ratio<3600>>(minutes).count();
}
}

auto format_durations(DurationMap durations) -> std::string
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

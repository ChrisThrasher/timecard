#include "FormatDurations.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

static auto longest_label(const DurationMap& durations)
{
    return static_cast<int>(
        std::max_element(std::begin(durations), std::end(durations), [](const auto& lhs, const auto& rhs) {
            return lhs.first.length() < rhs.first.length();
        })->first.length());
}

auto format_durations(DurationMap durations) -> std::string
{
    const auto off_time = durations.find("-") != durations.end() ? durations["-"] : Hours(0);
    durations.erase("-");

    const auto label_width = longest_label(durations) + 2;

    std::stringstream out;
    out << std::fixed << std::setprecision(1) << std::setfill(' ');

    auto total = Hours(0);
    for (const auto& duration : durations) {
        out << std::left << std::setw(label_width) << duration.first;
        out << std::right << std::setw(4) << duration.second.count() << " hours\n";
        total += duration.second;
    }

    out << "\nTotal: " << total.count() << " hours";
    if (off_time > std::chrono::minutes(0))
        out << " (" << off_time.count() << " hours off)";
    out << '\n';

    return out.str();
}

#pragma once

#include <DurationMap.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

int LongestLabel(const DurationMap& durations)
{
    return static_cast<int>(
        std::max_element(std::begin(durations), std::end(durations), [](const auto& lhs, const auto& rhs) {
            return lhs.first.length() < rhs.first.length();
        })->first.length());
}

auto FormatDurations(DurationMap durations)
{
    const auto off_time = durations.find("-") != durations.end() ? durations["-"] : Hours(0);
    durations.erase("-");

    const auto label_width = LongestLabel(durations) + 2;
    constexpr char separator = ' ';
    constexpr auto duration_width = 4;

    std::stringstream out;
    out << std::fixed << std::setprecision(1) << std::setfill(separator);

    auto total = Hours(0);
    for (const auto& duration : durations)
    {
        out << std::left << std::setw(label_width) << duration.first;
        out << std::right << std::setw(duration_width) << duration.second.count() << " hours\n";
        total += duration.second;
    }

    out << "\nTotal: " << total.count() << " hours";
    if (off_time > std::chrono::minutes(0))
        out << " (" << off_time.count() << " hours off)";
    out << '\n';

    return out.str();
}

#pragma once

#include <CalculateDurations.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

Hours OffTime(DurationMap& durations)
{
    const auto off_time_key = "-";
    const auto off_time_it = durations.find(off_time_key);
    durations.erase(off_time_key);
    return (off_time_it != durations.end()) ? off_time_it->second : Hours(0);
}

int LongestLabel(const DurationMap& durations)
{
    return static_cast<int>(
        std::max_element(std::begin(durations), std::end(durations), [](const auto& lhs, const auto& rhs) {
            return lhs.first.length() < rhs.first.length();
        })->first.length());
}

auto FormatDurations(DurationMap durations)
{
    const auto off_time = OffTime(durations);

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

    return out;
}

void PrintDurations(const DurationMap& durations) { std::cout << FormatDurations(durations).str(); }

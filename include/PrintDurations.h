#pragma once

#include <CalculateDurations.h>

#include <algorithm>
#include <iomanip>
#include <iostream>

DurationType OffTime(DurationMap& durations)
{
    const auto off_time_key = "-";
    const auto off_time_it = durations.find(off_time_key);
    durations.erase(off_time_key);
    return (off_time_it != durations.end()) ? off_time_it->second : DurationType(0);
}

int LongestLabel(const DurationMap& durations)
{
    return static_cast<int>(
        std::max_element(std::begin(durations), std::end(durations), [](const auto& lhs, const auto& rhs) {
            return lhs.first.length() < rhs.first.length();
        })->first.length());
}

void PrintDurations(DurationMap durations)
{
    const auto off_time = OffTime(durations);

    const char separator = ' ';
    const auto label_width = LongestLabel(durations) + 2;
    const auto duration_width = 4;
    std::cout << std::fixed << std::setprecision(1) << std::setfill(separator);

    auto total = DurationType(0);
    for (const auto& duration : durations)
    {
        std::cout << std::left << std::setw(label_width) << duration.first;
        std::cout << std::right << std::setw(duration_width) << duration.second.count() << " hours\n";
        total += duration.second;
    }

    std::cout << "\nTotal: " << total.count() << " hours";
    if (off_time > std::chrono::minutes(0))
        std::cout << " (" << off_time.count() << " hours off)";
    std::cout << '\n';
}

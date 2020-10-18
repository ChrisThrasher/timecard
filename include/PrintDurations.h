#pragma once

#include <CalculateDurations.h>

#include <iomanip>
#include <iostream>

void PrintDurations(const DurationMap& durations)
{
    auto total_duration = std::chrono::duration<double, std::ratio<3600>>(0);
    std::cout << std::fixed << std::setprecision(1);
    for (const auto& duration : durations)
    {
        if (duration.first == "-")
            continue;
        std::cout << duration.first << ": " << duration.second.count() << " hours\n";
        total_duration += duration.second;
    }
    std::cout << "\nTotal: " << total_duration.count() << " hours\n";
}

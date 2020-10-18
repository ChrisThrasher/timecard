#pragma once

#include <CalculateDurations.h>

#include <iomanip>
#include <iostream>

void PrintDurations(const DurationMap& durations)
{
    std::cout << std::fixed << std::setprecision(1);

    auto total = std::chrono::duration<double, std::ratio<3600>>(0);
    auto off_time = std::chrono::duration<double, std::ratio<3600>>(0);

    for (const auto& duration : durations)
    {
        if (duration.first == "-")
        {
            off_time += duration.second;
            continue;
        }
        std::cout << duration.first << ":\t" << duration.second.count() << " hours\n";
        total += duration.second;
    }

    std::cout << "\nTotal:\t" << total.count() << " hours";
    if (off_time > std::chrono::minutes(0))
    {
        std::cout << " (" << off_time.count() << " hours off)";
    }
    std::cout << '\n';
}

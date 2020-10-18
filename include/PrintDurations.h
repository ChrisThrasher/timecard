#pragma once

#include <CalculateDurations.h>

#include <iomanip>
#include <iostream>

void PrintDurations(const DurationMap& durations)
{
    std::cout << std::fixed << std::setprecision(1);
    for (const auto& duration : durations)
    {
        if (duration.first == "-")
            continue;
        std::cout << duration.first << ": " << duration.second.count() << " hours\n";
    }
}

#pragma once

#include <CalculateDurations.h>

#include <iomanip>
#include <iostream>

/*
    cout << left << setw(nameWidth) << setfill(separator) << "Bob";
    cout << left << setw(nameWidth) << setfill(separator) << "Doe";
    cout << left << setw(numWidth) << setfill(separator) << 10.96;
    cout << left << setw(numWidth) << setfill(separator) << 7.61;
    cout << left << setw(numWidth) << setfill(separator) << 14.39;
    cout << left << setw(numWidth) << setfill(separator) << 2.11;
    cout << left << setw(numWidth) << setfill(separator) << 47.30;
    cout << left << setw(numWidth) << setfill(separator) << 14.21;
    cout << left << setw(numWidth) << setfill(separator) << 44.58;
    cout << left << setw(numWidth) << setfill(separator) << 5.00;
    cout << left << setw(numWidth) << setfill(separator) << 60.23;
    cout << endl;
*/

int LongestLabel(const DurationMap& durations)
{
    size_t longest_label = 0;
    for (const auto& duration : durations)
    {
        longest_label = std::max(longest_label, duration.first.length());
    }
    return static_cast<int>(longest_label);
}

void PrintDurations(const DurationMap& durations)
{
    const char separator = ' ';
    const auto label_width = LongestLabel(durations) + 2;
    const auto duration_width = 4;
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
        std::cout << std::left << std::setw(label_width) << std::setfill(separator) << duration.first;
        std::cout << std::right << std::setw(duration_width) << std::setfill(separator) << duration.second.count()
                  << " hours\n";
        total += duration.second;
    }

    std::cout << "\nTotal: " << total.count() << " hours";
    if (off_time > std::chrono::minutes(0))
    {
        std::cout << " (" << off_time.count() << " hours off)";
    }
    std::cout << '\n';
}

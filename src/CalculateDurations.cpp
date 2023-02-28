#include <timecard/CalculateDurations.hpp>
#include <timecard/ParseTimepoint.hpp>

#include <stdexcept>
#include <string>

auto calculate_durations(const std::vector<std::string_view>& args) -> DurationMap
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

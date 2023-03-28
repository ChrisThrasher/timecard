#pragma once

#include <timecard/DurationMap.hpp>

#include <vector>

[[nodiscard]] auto calculate_durations(const std::vector<std::string_view>& args) -> DurationMap;

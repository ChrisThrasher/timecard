#pragma once

#include <timecard/DurationMap.hpp>

#include <vector>

auto calculate_durations(const std::vector<std::string>& args) -> DurationMap;

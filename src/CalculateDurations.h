#pragma once

#include <DurationMap.h>
#include <ParseTimepoint.h>

#include <vector>

auto calculate_durations(const std::vector<std::string>& args) -> DurationMap;

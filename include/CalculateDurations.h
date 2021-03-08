#pragma once

#include <DurationMap.h>
#include <ParseTimepoint.h>

#include <vector>

auto CalculateDurations(const std::vector<std::string>& args) -> DurationMap;

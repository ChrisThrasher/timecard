#pragma once

#include <chrono>
#include <string>

auto ParseTimepoint(const std::string& timepoint) -> std::chrono::minutes;

#pragma once

#include <chrono>
#include <string>

auto parse_timepoint(const std::string& timepoint) -> std::chrono::minutes;

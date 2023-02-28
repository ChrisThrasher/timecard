#pragma once

#include <chrono>
#include <string_view>

auto parse_timepoint(std::string_view timepoint) -> std::chrono::minutes;

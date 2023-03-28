#pragma once

#include <chrono>
#include <string_view>

[[nodiscard]] auto parse_timepoint(std::string_view timepoint) -> std::chrono::minutes;

#pragma once

#include <timecard/DurationMap.hpp>

[[nodiscard]] auto format_durations(DurationMap durations) -> std::string;

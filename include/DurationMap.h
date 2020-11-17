#pragma once

#include <chrono>
#include <map>
#include <string>

using Hours = std::chrono::duration<double, std::ratio<3600>>;
using DurationMap = std::map<std::string, Hours>;

#pragma once

#include <string>
#include <vector>

auto VectorizeArguments(const int argc, const char* const argv[]) -> std::vector<std::string>
{
    return std::vector<std::string>(argv + 1, argv + argc);
}

#pragma once

#include <string>
#include <vector>

auto VectorizeArguments(const int argc, char* argv[]) -> std::vector<std::string>
{
    std::vector<std::string> arguments;

    arguments.reserve(static_cast<size_t>(argc));
    for (int i = 0; i < argc; ++i)
    {
        arguments.push_back(std::string(argv[i]));
    }

    return arguments;
}

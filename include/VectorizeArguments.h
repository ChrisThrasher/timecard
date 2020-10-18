#pragma once

#include <string>
#include <vector>

auto VectorizeArguments(const int argc, char* argv[]) -> std::vector<std::string>
{
    std::vector<std::string> arguments;

    arguments.reserve(static_cast<size_t>(argc));
    for (int i = 1; i < argc; ++i) // Skip first element ignoring name of program
    {
        arguments.push_back(std::string(argv[i]));
    }

    return arguments;
}

#include <chrono>
#include <iostream>

auto ParseTimepoint(const std::string& timepoint)
{
    if (timepoint.size() != 4)
    {
        throw std::invalid_argument("Timepoint string must be 4 characters long");
    }

    const auto time = std::stoi(timepoint);
    const auto minutes = std::chrono::minutes(time % 100);
    const auto hours = std::chrono::hours((time - (time % 100)) / 100);

    return hours + minutes;
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::cout << ParseTimepoint(argv[i]).count() << " minutes\n";
    }
}

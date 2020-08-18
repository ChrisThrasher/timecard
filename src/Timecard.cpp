#include <CalculateDurations.h>
#include <CheckFlags.h>
#include <VectorizeArguments.h>

#include <iomanip>
#include <iostream>

int main(int argc, char* argv[])
try
{
    const auto args = VectorizeArguments(argc, argv);
    CheckFlags(args);

    const auto durations = CalculateDurations(args);

    std::cout << std::fixed << std::setprecision(1);
    for (const auto& duration : durations)
    {
        std::cout << duration.first << ": " << duration.second.count() << " hours\n";
    }
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << '\n';
    std::cerr << "Failed to parse arguments. Exiting.\n" << std::flush;
    return 1;
}
catch (...)
{
    std::cerr << "Failed to parse arguments. Exiting.\n" << std::flush;
    return 1;
}

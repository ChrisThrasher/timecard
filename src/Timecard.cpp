#include <CalculateDurations.h>
#include <CheckFlags.h>
#include <PrintDurations.h>
#include <VectorizeArguments.h>

int main(int argc, char* argv[])
try
{
    const auto args = VectorizeArguments(argc, argv);
    CheckFlags(args);
    PrintDurations(CalculateDurations(args));
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

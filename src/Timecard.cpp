#include <CalculateDurations.h>
#include <CheckFlags.h>
#include <PrintDurations.h>
#include <VectorizeArguments.h>

int main(const int argc, const char* const argv[])
try
{
    const auto args = VectorizeArguments(argc, argv);
    CheckFlags(args);
    PrintDurations(CalculateDurations(args));
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}

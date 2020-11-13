#include <CalculateDurations.h>
#include <CheckFlags.h>
#include <FormatDurations.h>
#include <VectorizeArguments.h>

int main(int argc, char* argv[])
try
{
    const auto args = VectorizeArguments(argc, argv);
    CheckFlags(args);
    std::cout << FormatDurations(CalculateDurations(args));
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
}

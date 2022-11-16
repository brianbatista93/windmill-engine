#include "Containers/ArrayBenchmark.h"
#include "Containers/SetBenchmark.h"
#include "Containers/StringBenchmark.h"
#include "HAL/PathBenchmark.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest();
    if (RUN_ALL_TESTS() != 0)
    {
        return 1;
    }
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        return 1;
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
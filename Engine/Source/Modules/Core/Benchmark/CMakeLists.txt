project ("Benchmark_Core")

add_executable (${PROJECT_NAME} "CoreUTests.cpp" "Containers/SetBenchmark.h" "HAL/PathBenchmark.h" "../HAL/FileSystem.cpp")

target_link_libraries (${PROJECT_NAME} benchmark::benchmark gtest
	Core
)

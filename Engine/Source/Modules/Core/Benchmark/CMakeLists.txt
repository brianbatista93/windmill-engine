project ("Benchmark_Core")

add_executable (${PROJECT_NAME} "CoreUTests.cpp")

target_link_libraries (${PROJECT_NAME}
PRIVATE
	Core
	benchmark::benchmark gtest
)

include (GoogleTest)
gtest_add_tests (${PROJECT_NAME} "" AUTO)

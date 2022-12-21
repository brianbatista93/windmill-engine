project ("Benchmark_Core")

add_executable (${PROJECT_NAME} "CoreUTests.cpp")

target_link_libraries (${PROJECT_NAME}
PRIVATE
	Core
	benchmark::benchmark gtest
)

add_test (NAME ${PROJECT_NAME}_gtests COMMAND ${PROJECT_NAME})

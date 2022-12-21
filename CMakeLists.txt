cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

option (ENABLE_TESTS_BENCHMARK "Enable tests and benchmark" OFF)

if (ENABLE_TESTS_BENCHMARK)
	include(CTest)
	enable_testing()
endif()

add_subdirectory("Engine")

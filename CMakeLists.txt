cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

include ("Build/CMake/CXX-Properties.cmake")
include ("Build/CMake/Config-Properties.cmake")

option (ENABLE_TESTS_BENCHMARK "Enable tests and benchmark" ON)
if (ENABLE_TESTS_BENCHMARK)
	set (BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
	add_subdirectory ("Source/ThirdParty/benchmark")
endif()

include ("Build/CMake/MSVC-Flags.cmake")

add_subdirectory ("Source/Modules/Core")

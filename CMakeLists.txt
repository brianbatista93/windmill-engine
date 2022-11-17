cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

option (ENABLE_TESTS_BENCHMARK "Enable tests and benchmark" ON)
if (ENABLE_TESTS_BENCHMARK)
	set (BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
	add_subdirectory ("Source/ThirdParty/benchmark")
endif()

if (WINDOWS)
    include ("Build/CMake/MSVC-Flags.cmake")
elseif (LINUX)
    include ("Build/CMake/GCC-Flags.cmake")
endif ()

include ("Build/CMake/CXX-Properties.cmake")
include ("Build/CMake/Config-Properties.cmake")

add_subdirectory ("Source/Modules/Core")
add_subdirectory ("Source/Modules/Engine")
add_subdirectory ("Source/Launchers")

add_subdirectory ("Source/Modules/${PLATFORM_NAME}")

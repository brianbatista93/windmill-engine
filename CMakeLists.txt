cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

include ("Build/CMake/CXX-Properties.cmake")

option (ENABLE_TESTS_BENCHMARK "Enable tests and benchmark" ON)
if (ENABLE_TESTS_BENCHMARK)
	set (BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
	add_subdirectory ("Source/ThirdParty/benchmark")
endif()

add_definitions (-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)
add_definitions (-DWE_DEBUG)
add_definitions (-DWE_OS_WINDOWS)

if (MSVC)
    add_compile_options (/W4 /WX)
    add_compile_options (/Zi)
    add_link_options (/DEBUG)
else()
    add_compile_options (-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory ("Source/Modules/Core")

cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option (ENABLE_TESTS_BENCHMARK "Enable tests and benchmark" OFF)
if (ENABLE_TESTS_BENCHMARK)
	set (BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
	add_subdirectory ("Source/ThirdParty/benchmark")
endif()

include ("Build/CMake/Engine-Flags.cmake")

if (WINDOWS)
    include ("Build/CMake/MSVC-Flags.cmake")
elseif (LINUX)
    include ("Build/CMake/GCC-Flags.cmake")
endif ()

include ("Build/CMake/CXX-Properties.cmake")
include ("Build/CMake/Config-Properties.cmake")

add_subdirectory ("Source/Modules/Core")
add_subdirectory ("Source/Modules/${PLATFORM_NAME}")

option (BUILD_TOOLS "Build tools" ON)
if (BUILD_TOOLS)
	add_subdirectory ("Source/Tools/Scribe")
endif ()

include ("Build/CMake/Build-Helper.cmake")

add_subdirectory ("Source/Modules/Engine")
add_subdirectory ("Source/Launchers")

add_custom_target (
	"Copy-Engine-Resources" ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Assets" "${CMAKE_BINARY_DIR}/Assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Config" "${CMAKE_BINARY_DIR}/Config"
	COMMENT "Coping engine resources..."
)
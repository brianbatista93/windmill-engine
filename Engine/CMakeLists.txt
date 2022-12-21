set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (ENABLE_TESTS_BENCHMARK)
	set (BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
	set (BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF)
    set (BENCHMARK_ENABLE_LIBPFM OFF)
    set (GTEST_COMPILE_COMMANDS OFF)
	add_subdirectory ("Source/ThirdParty/benchmark")
endif()

include ("Build/CMake/Engine-Flags.cmake")

option (ENABLE_ASAN "Enable Address sanitizer" OFF)
option (ENABLE_UBSAN "Enable Undefined Behavior sanitizer" OFF)
if (ENABLE_ASAN)
	message (STATUS "AddressSanitizer enabled")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
	set (CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} -fsanitize=address")
endif ()

if (ENABLE_UBSAN)
  message (STATUS "Undefined Behavior sanitizer enabled")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined,shift,shift-exponent,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr,pointer-overflow,builtin -fno-sanitize-recover=all")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined,shift,shift-exponent,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr,pointer-overflow,builtin -fno-sanitize-recover=all")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

	include ("Build/CMake/Clang-Flags.cmake")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	include ("Build/CMake/GCC-Flags.cmake")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    include ("Build/CMake/MSVC-Flags.cmake")
endif()

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
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Engine/Assets" "${CMAKE_BINARY_DIR}/Assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Engine/Config" "${CMAKE_BINARY_DIR}/Config"
	COMMENT "Coping engine resources..."
)

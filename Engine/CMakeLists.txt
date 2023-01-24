set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set ("YAML_BUILD_SHARED_LIBS=OFF")
add_subdirectory ("Source/ThirdParty/yaml-cpp")

include ("Build/CMake/Engine-Flags.cmake")

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
	if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
		include ("Build/CMake/MSVC-Clang-Flags.cmake")
	elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
		include ("Build/CMake/GNU-Clang-Flags.cmake")
	else ()
		message (FATAL_ERROR "Invalid Cland frontend variant")
	endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	include ("Build/CMake/GCC-Flags.cmake")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    include ("Build/CMake/MSVC-Flags.cmake")
endif()

include ("Build/CMake/CXX-Properties.cmake")
include ("Build/CMake/Config-Properties.cmake")

include ("Build/CMake/Build-Helper.cmake")

add_subdirectory ("Source/Modules/Core")
add_subdirectory ("Source/Modules/${PLATFORM_NAME}")

option (BUILD_TOOLS "Build tools" ON)
if (BUILD_TOOLS)
	add_subdirectory ("Source/Tools/Scribe")
endif ()

add_subdirectory ("Source/Modules/Engine")
add_subdirectory ("Source/Launchers")

add_custom_target (
	"Copy-Engine-Resources" ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Engine/Assets" "${CMAKE_BINARY_DIR}/Assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Engine/Config" "${CMAKE_BINARY_DIR}/Config"
	COMMENT "Coping engine resources..."
)

set_target_properties ("Copy-Engine-Resources" PROPERTIES
  FOLDER "Helpers"
)

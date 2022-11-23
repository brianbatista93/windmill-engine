project ("Core")

add_library (${PROJECT_NAME} STATIC
 "Types.hpp"
 "Memory/MemoryManager.hpp"
 "Memory/MemoryManager.cpp"
 "Memory/Memory.hpp"
 "Math/MathUtils.hpp"
 "Containers/Allocator.hpp"
 "Containers/Array.hpp"
 "Containers/WeString.hpp" 
 "Containers/String.cpp"
 "Containers/StringUtils.cpp"
 "Containers/StringBuilder.cpp"
 "Encoding/AnsiEncoder.cpp" 
 "Containers/StringFormat.cpp"
 "Containers/StringFormatter.cpp" 
 "HAL/Path.cpp"
 "HAL/FileSystem.cpp"
 "Globals.cpp" "HAL/File.cpp" "Encoding/UTF8Encoder.cpp")

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries (${PROJECT_NAME}
PUBLIC
    ${PLATFORM_NAME}
)

if (ENABLE_TESTS_BENCHMARK)
	add_subdirectory ("Benchmark")
endif ()
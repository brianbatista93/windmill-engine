project ("Core")

add_library (${PROJECT_NAME} STATIC
 "Containers/String.cpp"
 "Containers/StringBuilder.cpp"
 "Containers/StringFormat.cpp"
 "Containers/StringFormatter.cpp" 
 "Containers/StringUtils.cpp"
 "Encoding/AnsiEncoder.cpp" 
 "Encoding/UTF8Encoder.cpp"
 "Globals.cpp"
 "HAL/File.cpp"
 "HAL/FileSystem.cpp"
 "HAL/Path.cpp"
 "Logging/Log.cpp"
 "Logging/LogFileSink.cpp"
 "Logging/StdLogSink.cpp"
 "Memory/MemoryManager.cpp" "Console/ConfigFile.cpp")

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries (${PROJECT_NAME}
PUBLIC 
	"${PLATFORM_NAME}"
)

if (ENABLE_TESTS_BENCHMARK)
	add_subdirectory ("Benchmark")
endif ()

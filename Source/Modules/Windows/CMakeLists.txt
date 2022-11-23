project ("Windows")

add_library (${PROJECT_NAME} STATIC 
 "HAL/WindowsFileSystem.cpp"
 "OS/WindowsStringConvertion.cpp" "OS/WindowsUtils.cpp")

target_link_libraries (${PROJECT_NAME}
PRIVATE
    "Core"
)

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
project ("Linux")

add_library (${PROJECT_NAME} STATIC 
    "HAL/LinuxFileSystem.cpp"
)

target_link_libraries (${PROJECT_NAME}
PRIVATE
    "Core"
)

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
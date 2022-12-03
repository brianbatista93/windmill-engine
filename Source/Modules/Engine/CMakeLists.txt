project ("Engine")

add_library (${PROJECT_NAME} STATIC
 "Engine.cpp"
 "Classes/WTexture.cpp")

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${WE_GENERATED_DIRECTORY}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries (${PROJECT_NAME}
PUBLIC
    "Core"
)
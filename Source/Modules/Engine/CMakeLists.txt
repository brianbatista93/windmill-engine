project ("Engine")

add_library (${PROJECT_NAME} STATIC
 "Engine.cpp"
)

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries (${PROJECT_NAME}
PUBLIC
    "Core"
)
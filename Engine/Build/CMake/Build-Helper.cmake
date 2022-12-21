function (scribe_module project_name)
#	add_custom_command (
#		TARGET ${project_name}
#		PRE_BUILD
#		COMMENT "Running Scribe for module: ${project_name}"
#		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#		COMMAND ${CMAKE_BINARY_DIR}/Scribe${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_SOURCE_DIR} -o ${CMAKE_BINARY_DIR}/Generated/${project_name} -l ${CMAKE_SOURCE_DIR}/LICENSE
#		VERBATIM
#	)

	add_dependencies (${project_name} Scribe)

	target_include_directories (${project_name}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/${project_name}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
endfunction()

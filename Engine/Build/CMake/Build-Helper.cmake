if(BUILDER_HELPER_INCLUDED)
  message(WARNING "Builder-Helper.cmake has already been included!!")
else()
  set(BUILDER_HELPER_INCLUDED 1)
endif()

include ("Build/CMake/Clang-Linter.cmake")

macro (_add_scribe_module project_name)
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
      PRIVATE
          $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Generated/${project_name}>
    )
endmacro()

macro (_add_project project_name is_tool)
  set(options IS_SCRIBE_PROJECT)
  set(oneValueArgs VERSION DESCRIPTION)
  set(multiValueArgs PUBLIC PRIVATE DEPENDS)

  cmake_parse_arguments(_WE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (_WE_VERSION)
    project (${project_name} VERSION "${_WE_VERSION}" DESCRIPTION "${_WE_DESCRIPTION}")
  else ()
    project (${project_name} DESCRIPTION "${_WE_DESCRIPTION}")
  endif ()

  if (NOT _WE_PUBLIC AND NOT _WE_PRIVATE)
    message (FATAL_ERROR "You must specify PUBLIC or PRIVATE source files for project ${project_name}")
  endif ()

  if (${is_tool})
    add_executable (${project_name})
    target_compile_options (${project_name} PRIVATE
      $<$<CXX_COMPILER_ID:GNU>:-fexceptions>
    )
    set (FOLDER_NAME "Tool")
  else ()
    add_library (${project_name} STATIC)
    set (FOLDER_NAME "Module")
    target_compile_definitions (${project_name} PRIVATE WE_IS_MODULE)
  endif ()
  
  target_sources (${project_name}
    PRIVATE
      ${_WE_PRIVATE}
      ${_WE_PUBLIC}
  )
  
  target_include_directories (${project_name}
	PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  )

  if (${_WE_IS_SCRIBE_PROJECT})
    _add_scribe_module (${project_name})
  endif()
  
  if (${ENABLE_WINDMILL_TESTS} AND EXISTS "${PROJECT_SOURCE_DIR}/Tests")
    add_subdirectory ("Tests")
  endif ()

  set_target_properties (${project_name} PROPERTIES
    FOLDER "${FOLDER_NAME}s"
  )

  target_link_libraries (${project_name}
    PUBLIC
      ${_WE_DEPENDS}
  )

  set (SOURCE_FILES ${_WE_PRIVATE})

  add_linter (${project_name})
  
  if (_WE_VERSION)
    message (STATUS "${FOLDER_NAME} \"${project_name}-v${PROJECT_VERSION}\" added to build.")
  else ()
    message (STATUS "${FOLDER_NAME} \"${project_name}\" added to build.")
  endif ()
endmacro ()

function (add_we_module module_name)
  _add_project (${module_name} FALSE ${ARGN})
endfunction ()

function (add_we_tool tool_name)
  _add_project (${tool_name} TRUE ${ARGN})
endfunction ()

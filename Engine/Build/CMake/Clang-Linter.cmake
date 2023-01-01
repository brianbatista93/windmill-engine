if(CLANG_LINTER_INCLUDED)
  message(WARNING "Clang-Linter.cmake has already been included!!")
else()
  set(CLANG_LINTER_INCLUDED 1)
endif()

add_custom_target (LintProjects
  EXCLUDE_FROM_ALL
)

set_target_properties (LintProjects PROPERTIES FOLDER "Helpers")

function (add_linter target_name)
  set(lint_timestamp "${CMAKE_BINARY_DIR}/${target_name}/.lint-timestamp")
  message (STATUS ${lint_timestamp})
  if (CMAKE_COMPILER_IS_GNUCXX)
    set (COMPILER_OPTIONS -fno-caret-diagnostics)
  else()
    set (COMPILER_OPTIONS)
  endif()
  add_custom_command(
    OUTPUT ${lint_timestamp}
    COMMAND clang-tidy
            "--warnings-as-errors='*'"
            --quiet
            -p ${CMAKE_BINARY_DIR}
            -export-fixes="${CMAKE_BINARY_DIR}/${target_name}-clang-fixes.yaml"
            --extra-arg="${COMPILER_OPTIONS}"
            ${SOURCE_FILES}
    COMMAND "${CMAKE_COMMAND}" -E touch ${lint_timestamp}
    COMMENT "Linting... ${target_name}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  add_custom_target(${target_name}Lint DEPENDS ${lint_timestamp})
  set_target_properties (${target_name}Lint PROPERTIES FOLDER "Helpers")
  add_dependencies (LintProjects ${target_name}Lint)
endfunction()

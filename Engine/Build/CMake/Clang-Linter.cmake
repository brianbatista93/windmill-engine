function (add_linter target_name)
  set(lint_timestamp "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.__lint_timestamp__")
  message (STATUS ${lint_timestamp})
  add_custom_command(
    OUTPUT ${lint_timestamp}
    COMMAND clang-tidy
            "--warnings-as-errors='*'"
            --quiet
            -p ${PROJECT_BINARY_DIR}
            -export-fixes="${CMAKE_CURRENT_BINARY_DIR}/${target_name}-clang-fixes.yaml"
            ${SOURCE_FILES}
    COMMAND "${CMAKE_COMMAND}" -E touch ${lint_timestamp}
    COMMENT "Linting... ${target_name}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  add_custom_target(${target_name}.__lint__
      DEPENDS ${lint_timestamp})
endfunction()

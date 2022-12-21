project ("Launch")

add_executable (${PROJECT_NAME} WIN32 
    "${PLATFORM_NAME}/${PLATFORM_NAME}Launcher.cpp"
    "Launcher.cpp"
)

target_link_libraries (${PROJECT_NAME} PRIVATE
    "Engine"
)

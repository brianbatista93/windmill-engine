add_executable (${PROJECT_NAME} WIN32 
    "WindowsLauncher.cpp"
    "../Launcher.cpp"
)

target_link_libraries (${PROJECT_NAME} PRIVATE
    "Core"
    "Engine"
)
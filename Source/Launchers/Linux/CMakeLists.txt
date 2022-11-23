add_executable (${PROJECT_NAME} 
    "LinuxLauncher.cpp"
    "../Launcher.cpp"
)

target_link_libraries (${PROJECT_NAME} PRIVATE
    "Core"
    "Engine"
)
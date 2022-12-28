project ("Launch")

add_executable (${PROJECT_NAME} WIN32 
    "${PLATFORM_NAME}/${PLATFORM_NAME}Launcher.cpp"
    "Launcher.cpp"
)

target_link_libraries (${PROJECT_NAME} PRIVATE
    "Engine"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DPI_AWARE "PerMonitor")

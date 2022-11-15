project ("Launch")

if (WINDOWS)
    add_subdirectory ("Windows")
elseif (LINUX)
    add_subdirectory ("Linux")
endif ()
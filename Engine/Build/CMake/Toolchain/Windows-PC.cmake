set (CMAKE_SYSTEM_NAME "Windows")
set (CMAKE_SYSTEM_VERSION 10.0.22621.0)
set (WINDOWS TRUE)
set (PLATFORM_NAME "Windows")
set (OUTPUT_DIRECTORY_NAME "Win64")

add_definitions (-D_WINDOWS -DWIN32 -D_WIN32 -D_WIN64)

add_definitions (-DWE_OS_WINDOWS)
add_definitions (-DWE_OS_ALIGNED_ALLOCATION)
add_definitions (-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)
add_definitions (-DWE_OS_MAX_PATH=260)

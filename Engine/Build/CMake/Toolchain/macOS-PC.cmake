set (CMAKE_SYSTEM_NAME "Darwin")
set (DARWIN TRUE)
set (PLATFORM_NAME "MacOS")
set (OUTPUT_DIRECTORY_NAME "MacOS")

add_definitions (-DWE_OS_MACOS)
add_definitions (-DUNICODE -D_UNICODE)
add_definitions (-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)
add_definitions (-DWE_OS_MAX_PATH=512)
set (CMAKE_SYSTEM_NAME "WindowsStore")
set (CMAKE_SYSTEM_VERSION 10.0.22621.0)
<<<<<<<< HEAD:Engine/Build/CMake/Toolchain/WindowsPC-MSVC.cmake

set (BUILD_CPU_ARCHITECTURE x64)
set (BUILD_PLATFORM Win64)
========
>>>>>>>> main:Engine/Build/CMake/Toolchain/Windows-Store.cmake
set (WINDOWS TRUE)
set (PLATFORM_NAME "Windows")
set (OUTPUT_DIRECTORY_NAME "UWP")

add_definitions (-D_WINDOWS -DWIN32 -D_WIN32 -D_WIN64 -DUNICODE -D_UNICODE)

add_definitions (-DWE_OS_WINDOWS)
add_definitions (-DWE_OS_UWP)
add_definitions (-DWE_OS_ALIGNED_ALLOCATION)
add_definitions (-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)
add_definitions (-DWE_OS_MAX_PATH=260)

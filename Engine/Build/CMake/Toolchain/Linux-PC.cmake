set (CMAKE_SYSTEM_NAME "Linux")
set (LINUX TRUE)
set (PLATFORM_NAME "Linux")
set (OUTPUT_DIRECTORY_NAME "Linux64")

if (NOT CMAKE_C_COMPILER)
	set (CMAKE_C_COMPILER gcc)
endif ()

if (NOT CMAKE_CXX_COMPILER)
	set (CMAKE_CXX_COMPILER g++)
endif ()

add_definitions (-DWE_OS_LINUX)
add_definitions (-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)
add_definitions (-DWE_OS_MAX_PATH=512)

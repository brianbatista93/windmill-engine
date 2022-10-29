cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

include ("Build/CMake/CXX-Properties.cmake")

# Move this to another file

add_definitions (-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)

if (MSVC)
    add_compile_options (/W4 /WX)
else()
    add_compile_options (-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory ("Source/Modules/Core")
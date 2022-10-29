cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Move this to another file

add_definitions(-DWE_OS_MEM_DEFAULT_ALIGNMENT=16)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory ("Source/Modules/Core")
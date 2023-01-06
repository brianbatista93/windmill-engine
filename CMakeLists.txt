cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

option (ENABLE_WINDMILL_TESTS "Enable unit tests" ON)
option (ENABLE_ASAN "Enable Address sanitizer" OFF)
option (ENABLE_UBSAN "Enable Undefined Behavior sanitizer" OFF)

if (ENABLE_WINDMILL_TESTS)
	include(CTest)
	enable_testing()
    
    set (BUILD_GMOCK OFF)
    set (INSTALL_GTEST OFF)
    set (gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
	add_subdirectory ("Engine/Source/ThirdParty/googletest")
endif()

add_subdirectory("Engine")

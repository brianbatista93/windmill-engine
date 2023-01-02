cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

option (ENABLE_WINDMILL_TESTS "Enable unit tests" OFF)
option (ENABLE_ASAN "Enable Address sanitizer" OFF)
option (ENABLE_UBSAN "Enable Undefined Behavior sanitizer" OFF)

if (ENABLE_WINDMILL_TESTS)
	include(CTest)
	enable_testing()
endif()

add_subdirectory("Engine")

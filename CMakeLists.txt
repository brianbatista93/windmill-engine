cmake_minimum_required (VERSION 3.16)

project (WindmillEngine)

option (ENABLE_WINDMILL_TESTS "Enable unit tests" ON)

if (ENABLE_WINDMILL_TESTS)
	include(CTest)
	enable_testing()
endif()

add_subdirectory("Engine")

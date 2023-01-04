project ("UnitTest_Core")

add_executable (${PROJECT_NAME} "CoreUTests.cpp")

target_link_libraries (${PROJECT_NAME}
PRIVATE
	Core
    gtest_main
)

add_test (NAME ${PROJECT_NAME}_gtests COMMAND ${PROJECT_NAME})

set_target_properties (${PROJECT_NAME} PROPERTIES
  FOLDER "Tests"
)

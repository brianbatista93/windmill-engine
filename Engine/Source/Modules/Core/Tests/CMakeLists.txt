project ("UnitTest_Core")

add_executable (${PROJECT_NAME}
  "Containers/ArrayBenchmark.cc"
  "Containers/SetBenchmark.cc"
  "Containers/StringBenchmark.cc"
  "HAL/PathBenchmark.cc"
  "HAL/TimestampTest.cc"
)

target_link_libraries (${PROJECT_NAME}
PRIVATE
	Core
    gtest_main
)

add_test (NAME ${PROJECT_NAME}_gtests COMMAND ${PROJECT_NAME})

set_target_properties (${PROJECT_NAME} PROPERTIES
  FOLDER "Tests"
)

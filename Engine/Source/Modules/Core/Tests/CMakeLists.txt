project ("UnitTest_Core")

add_executable (${PROJECT_NAME}
  "Containers/ArrayBenchmark.cc"
  "Containers/SetBenchmark.cc"
  "Containers/StringBenchmark.cc"
  "HAL/PathBenchmark.cc"
  "HAL/TimestampTest.cc"
  "Misc/ULIDTest.cc"
)

target_link_libraries (${PROJECT_NAME}
  PRIVATE
	  Core
      gtest_main
)

target_include_directories (${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_compile_options (${PROJECT_NAME}
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd6326>
    $<$<CXX_COMPILER_ID:GNU>:-Wno-unused-parameter>
    $<$<CXX_COMPILER_ID:Clang>:-Wno-unused-parameter>
)

add_test (NAME ${PROJECT_NAME}_gtests COMMAND ${PROJECT_NAME})

set_target_properties (${PROJECT_NAME} PROPERTIES
  FOLDER "Tests"
)

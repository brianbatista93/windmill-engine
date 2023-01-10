project ("UnitTest_Core")

add_executable (${PROJECT_NAME}
  "Containers/ArrayBenchmark.cc"
  "Containers/SetBenchmark.cc"
  "Containers/StringBenchmark.cc"
  "HAL/PathBenchmark.cc"
  "HAL/TimestampTest.cc"
  "Misc/ULIDTest.cc"
  "Serialization/ArchiveTest.cc"
)

target_link_libraries (${PROJECT_NAME}
  PRIVATE
	  Core
      gtest_main
      gmock_main
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

set_target_properties (${PROJECT_NAME} PROPERTIES
  FOLDER "Tests"
)

include(GoogleTest)
gtest_add_tests (${PROJECT_NAME} "" AUTO)

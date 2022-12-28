add_we_module ("Windows"
  VERSION "0.0.1"
  DESCRIPTION "Engine module"
  DEPENDS
    "Core"
  PUBLIC
    "WindowsInclude.hpp"
    "HAL/WindowsFileSystem.hpp"
    "Logging/WindowsOutputLogSink.hpp"
  PRIVATE
    "HAL/WindowsFileSystem.cpp"
    "OS/WindowsStringConvertion.cpp" 
    "OS/WindowsUtils.cpp" 
    "OS/WindowsOS.cpp" 
    "Logging/WindowsOutputLogSink.cpp"
)

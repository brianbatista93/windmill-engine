add_we_module ("Windows"
  VERSION "0.0.1"
  DESCRIPTION "Engine module"
  DEPENDS
    "Core"
  PRIVATE
    "WindowsInclude.hpp"
    "HAL/WindowsTimestamp.hpp"
    "HAL/WindowsFileSystem.hpp"
    "Logging/WindowsOutputLogSink.hpp"
    "HAL/WindowsFileSystem.cpp"
    "HAL/WindowsTimestamp.cpp"
    "HAL/WindowsPaths.cpp"
    "OS/WindowsStringConvertion.cpp" 
    "OS/WindowsUtils.cpp" 
    "OS/WindowsOS.cpp" 
    "Logging/WindowsOutputLogSink.cpp"
)

add_we_module ("Linux"
  VERSION "0.0.1"
  DESCRIPTION "Engine module"
  DEPENDS
    "Core"
  PRIVATE
    "OS/LinuxSC.cpp"
    "OS/LinuxOS.cpp"
    "HAL/LinuxFileSystem.cpp"
)

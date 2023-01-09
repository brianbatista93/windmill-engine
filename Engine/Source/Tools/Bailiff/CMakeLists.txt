add_we_tool ("Bailiff"
  VERSION "0.0.1"
  DESCRIPTION "Windmill Engine CLI"
  DEPENDS
    "Core"
    "yaml-cpp"
  PUBLIC
  PRIVATE
    "Project/BailiffProject.hpp"
    "Project/BailiffProject.cpp"
    "Exceptions/BailiffException.hpp"
    "Main.cpp"
)

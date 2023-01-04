add_we_module ("Engine"
  VERSION "0.0.1"
  DESCRIPTION "Engine module"
  IS_SCRIBE_PROJECT
  DEPENDS
    "Core"
  PUBLIC
    "Engine.hpp"
    "Classes/Texture.hpp"
  PRIVATE
    "Engine.cpp"
    "Classes/Texture.cpp"
)

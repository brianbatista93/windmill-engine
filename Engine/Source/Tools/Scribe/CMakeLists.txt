add_we_tool ("Scribe"
  VERSION "0.0.1"
  DESCRIPTION "Engine module"
  DEPENDS
    "Core"
  PUBLIC
    "Attributes.hpp"
    "Attributes/AttrProperty.hpp"
    "ScribeExceptions.hpp"
  PRIVATE
    "Main.cpp"
    "Processor.cpp"
    "Attributes.cpp"
    "Attributes/AttrProperty.cpp"
)

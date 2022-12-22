project ("Scribe")

add_executable (${PROJECT_NAME}
 "Main.cpp"
 "Processor.cpp"
 "Attributes.cpp"
 "Attributes.hpp"
 "Attributes/AttrProperty.cpp"
 "Attributes/AttrProperty.hpp"
 "ScribeExceptions.hpp"
)

target_link_libraries (${PROJECT_NAME}
PRIVATE
	"Core"
)

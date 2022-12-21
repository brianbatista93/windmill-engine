project ("Scribe")

add_executable (${PROJECT_NAME}
 "Main.cpp"
 "Processor.cpp"
 "Attributes.cpp"
 "Attributes/AttrProperty.cpp")

target_link_libraries (${PROJECT_NAME} PRIVATE
	"Core"
)

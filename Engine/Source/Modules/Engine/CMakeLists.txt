project ("Engine")

add_library (${PROJECT_NAME} STATIC
 "Engine.cpp"
 "Classes/WTexture.cpp")

target_link_libraries (${PROJECT_NAME}
PUBLIC
    "Core"
)

scribe_module (${PROJECT_NAME})
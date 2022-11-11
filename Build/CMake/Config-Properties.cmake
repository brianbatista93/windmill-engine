add_compile_definitions (
    $<$<CONFIG:Debug>:WE_DEBUG>
    $<$<CONFIG:Profile>:WE_PROFILE>    
    $<$<CONFIG:Release>:WE_RELEASE>
)
add_compile_definitions (
    $<$<CONFIG:Debug>:-DWE_DEBUG>
    $<$<CONFIG:Profile>:-DWE_PROFILE>    
    $<$<CONFIG:Release>:-DWE_RELEASE>
)
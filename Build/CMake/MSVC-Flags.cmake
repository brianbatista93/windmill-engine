add_compile_options (
 /nologo        # Suppress startup banner
 /fp:fast       # Use fast floating point model
 /Zc:wchar_t    # wchar_t is native type
 /GF            # Enable read-only string pooling
 /Gy            # Function-level linking
 /utf-8         # Use UTF-8 encoding for source code
 /Wv:19.30      # Set the Visual C++ version to 2022
 /MP            # Build with multiple processes
 /bigobj        # Increase object file max size

 /W4            # Set warning level 4
 /WX            # Treat warnings as errors
 /wd4267		# Disable warning C4267: 'argument' : conversion from 'size_t' to 'type', possible loss of data

 /DUNICODE      # Unicode character set
 /D_UNICODE     # Unicode character set
 /D_CRT_SECURE_NO_WARNINGS # Disable warnings about deprecated CRT functions
)

set (CMAKE_RC_FLAGS /nologo)

add_compile_options (
# Debug flags
    $<$<CONFIG:Debug>:/MDd>     # Use the debug multithread-specific and DLL-specific version of the run-time library.
    $<$<CONFIG:Debug>:/Zi>      # Enable debugging information
    $<$<CONFIG:Debug>:/Zo>      # Generate enhanced debugging information for optimized code in non-debug builds.
    $<$<CONFIG:Debug>:/Od>      # Disable optimizations
    $<$<CONFIG:Debug>:/Ob0>     # Disable inlining
    $<$<CONFIG:Debug>:/Oy->     # Disable suppressing of the creation of frame pointers on the call stack for quicker function calls
    $<$<CONFIG:Debug>:/RTC1>    # Enable run-time error checking
    $<$<CONFIG:Debug>:/GS>      # Enable buffer security checks
    $<$<CONFIG:Debug>:/DDEBUG>  # Define DEBUG
    $<$<CONFIG:Debug>:/D_DEBUG> # Define _DEBUG

# Profile flags
    $<$<CONFIG:Profile>:/MD>    # Use the multithread-specific and DLL-specific version of the run-time library.
    $<$<CONFIG:Profile>:/Zi>    # Enable debugging information
    $<$<CONFIG:Profile>:/Zo>    # Generate enhanced debugging information for optimized code in non-debug builds.
    $<$<CONFIG:Profile>:/Ox>    # Enable full optimizations
    $<$<CONFIG:Profile>:/Oy->   # Disable suppressing of the creation of frame pointers on the call stack for quicker function calls
    $<$<CONFIG:Profile>:/GS->   # Disable buffer security checks
    $<$<CONFIG:Profile>:/D_PROFILE> # Define PROFILE

# Release flags
    $<$<CONFIG:Release>:/MD>    # Use the multithread-specific and DLL-specific version of the run-time library.
    $<$<CONFIG:Release>:/Zi>    # Enable debugging information
    $<$<CONFIG:Release>:/Zo>    # Generate enhanced debugging information for optimized code in non-debug builds.
    $<$<CONFIG:Release>:/Ox>    # Enable full optimizations
    $<$<CONFIG:Release>:/GS->   # Disable buffer security checks
    $<$<CONFIG:Release>:/DNDEBUG> # Define NDEBUG
    $<$<CONFIG:Release>:/D_RELEASE> # Define _RELEASE
)

add_link_options (
    $<$<CONFIG:Profile>:/debug>         # Generate debug information
    $<$<CONFIG:Profile>:/INCREMENTAL>   # Enable incremental linking

    $<$<CONFIG:Release>:/debug>         # Generate debug information
    $<$<CONFIG:Release>:/OPT:REF>       # Eliminate unreferenced data
    $<$<CONFIG:Release>:/OPT:ICF>       # Enable COMDAT folding
)
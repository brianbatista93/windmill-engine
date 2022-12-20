add_compile_options (
 /arch:AVX	# Enable AVX instructions
 /bigobj        # Increase object file max size
 /D_CRT_SECURE_NO_WARNINGS # Disable warnings about deprecated CRT functions
 /D_DISABLE_EXTENDED_ALIGNED_STORAGE	# Disable aligned storage
 /DSAL_NO_ATTRIBUTE_DECLARATIONS=1	# Disable SAL attribute declarations
 /EHsc		# Enable C++ exceptions
 /errorReport:prompt	# Prompt for error report
 /fastfail	# Enable fastfail
 /FC			# Full path of source file in diagnostics
 /fp:fast       # Use fast floating point model
 /GL		# Whole program optimization
 /Gw			# Symbols for minimal rebuild
 /Gy            # Function-level linking
 /MP            # Build with multiple processes
 /nologo        # Suppress startup banner
 /Oi			# Enable intrinsic functions
 /permissive-	# Standards conformance mode
 /utf-8         # Use UTF-8 encoding for source code
 /W4            # Set warning level 4
 /wd4267		# Disable warning C4267: 'argument' : conversion from 'size_t' to 'type', possible loss of data
 /wd4819	# Disable warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss
 /wd5030		# Disable warning C5030: attribute 'attribute' is not recognized
 /Wv:19.30      # Set the Visual C++ version to 2022
 /WX            # Treat warnings as errors
 /Zc:__cplusplus	# Enable __cplusplus macro
 /Zc:char8_t	# char8_t is native type
 /Zc:enumTypes		# Enable C++17 enum class type deduction
 /Zc:inline		# Enable C++17 inline variables
 /Zc:preprocessor	# Enable C++17 compliant preprocessor
 /Zc:strictStrings- # Disable strict string literal conversion rules
 /Zc:wchar_t    # wchar_t is native type
 /Zm1000		# Set PCH memory threshold to 1000MB
)

set (CMAKE_RC_FLAGS /nologo)

add_compile_options (
# Debug flags
    $<$<CONFIG:Debug>:/MDd>					# Use the debug multithread-specific and DLL-specific version of the run-time library.
    $<$<CONFIG:Debug>:/Zi>					# Enable debugging information
    $<$<CONFIG:Debug>:/Zo>					# Generate enhanced debugging information for optimized code in non-debug builds.
    $<$<CONFIG:Debug>:/Od>					# Disable optimizations
	$<$<CONFIG:Debug>:/Os>					# Optimize for size
    $<$<CONFIG:Debug>:/Ob0>					# Disable inlining
    $<$<CONFIG:Debug>:/Oy->					# Disable suppressing of the creation of frame pointers on the call stack for quicker function calls
    $<$<CONFIG:Debug>:/RTC1>				# Enable run-time error checking
    $<$<CONFIG:Debug>:/GS>					# Enable buffer security checks
    $<$<CONFIG:Debug>:/DDEBUG>				# Define DEBUG
    $<$<CONFIG:Debug>:/D_DEBUG>				# Define _DEBUG
    $<$<CONFIG:Debug>:/analyze>				# Enable static code analysis
    $<$<CONFIG:Debug>:/fsanitize=address>	# Enable address sanitizer
    $<$<CONFIG:Debug>:/DFORCE_ANSI_ALLOCATOR=1> # Force use of ANSI allocator

# Profile flags
    $<$<CONFIG:Profile>:/MD>				# Use the multithread-specific and DLL-specific version of the run-time library.
    $<$<CONFIG:Profile>:/Zi>				# Enable debugging information
    $<$<CONFIG:Profile>:/Zo>				# Generate enhanced debugging information for optimized code in non-debug builds.
    $<$<CONFIG:Profile>:/Ox>				# Enable full optimizations
    $<$<CONFIG:Profile>:/Ot>				# Enable optimizations for speed
    $<$<CONFIG:Profile>:/Oy->				# Disable suppressing of the creation of frame pointers on the call stack for quicker function calls
    $<$<CONFIG:Profile>:/GS->				# Disable buffer security checks
    $<$<CONFIG:Profile>:/GF>				# Enable read-only string pooling
    $<$<CONFIG:Profile>:/D_PROFILE>			# Define PROFILE

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
    $<$<CONFIG:Debug>:/DEBUG:FULL>         # Generate debug information

    $<$<CONFIG:Profile>:/debug>         # Generate debug information
    $<$<CONFIG:Profile>:/INCREMENTAL>   # Enable incremental linking

    $<$<CONFIG:Release>:/debug>         # Generate debug information
    $<$<CONFIG:Release>:/OPT:REF>       # Eliminate unreferenced data
    $<$<CONFIG:Release>:/OPT:ICF>       # Enable COMDAT folding
)

add_compile_options (
    -ffast-math                     # Enable fast math optimizations
    -flax-vector-conversions        # Allow implicit conversions between vector types
    -fvisibility=hidden             # Hide symbols by default
    -fPIC                           # Position independent code
    -fno-exceptions                 # Disable exceptions
 
    -Wall                           # Enable all warnings
    -Werror                         # Treat warnings as errors
    -Wno-unknown-pragmas            # Disable unknown pragma warnings
	-Wno-attributes
	
    -fno-rtti                       # Disable RTTI
    -Wno-invalid-offsetof           # Disable invalid offsetof warnings
    -Wno-aligned-new                # Disable aligned new warnings
    -Wno-conversion-null            # Disable conversion null warnings
    -Wno-unused-result              # Disable unused result warnings
    -Wno-reorder                    # Disable reorder warnings
    -Wno-delete-non-virtual-dtor    # Disable delete non virtual dtor warnings
    -Wno-class-memaccess            # Disable class memaccess warnings
    -fpermissive                    # Enable permissive mode
	-fchar8_t						# Enable char8_t
)

add_compile_options (
# Debug flags
    $<$<CONFIG:Debug>:-g>								# Enable debug symbols
    $<$<CONFIG:Debug>:-O0>								# Disable optimizations
    $<$<CONFIG:Debug>:-DDEBUG>							# Define DEBUG macro
    $<$<CONFIG:Debug>:-D_DEBUG>							# Define _DEBUG macro
    
# Profile flags
    $<$<CONFIG:Profile>:-g>			# Enable debug symbols
    $<$<CONFIG:Profile>:-O2>		# Enable optimizations
    $<$<CONFIG:Profile>:-D_PROFILE>	# Define _PROFILE macro
    $<$<CONFIG:Profile>:-DNDEBUG>	# Define NDEBUG macro

# Release flags
    $<$<CONFIG:Release>:-O3>        # Enable optimizations
    $<$<CONFIG:Release>:-D_RELEASE> # Define _RELEASE macro
    $<$<CONFIG:Release>:-DNDEBUG>   # Define NDEBUG macro
)

set(CMAKE_LINK_FLAGS "{$CMAKE_LINK_FLAGS} -Wl, --gc-sections")

add_definitions (-DUNICODE -D_UNICODE)

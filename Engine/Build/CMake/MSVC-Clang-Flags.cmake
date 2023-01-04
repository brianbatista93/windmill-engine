add_compile_options (
	-fms-compatibility-version=1917 # Compatibility with VS 2022
	-fdiagnostics-absolute-paths	# Show absolute paths in diagnostics
	--target=x86_64-pc-windows-msvc	# Target Windows
	-mssse3							# Use SSSE3 instructions
	-mwaitpkg						# Use waitpkg instructions
	/Brepro							# Reproducible builds
	/fp:precise						# Use precise floating point
	-Wno-unknown-attributes

	/W4
	/WX

	-Werror
	-Wno-microsoft
	-Wno-msvc-include
	-Wno-ignored-qualifiers
	-Wno-missing-field-initializers
	-Wno-unused-private-field
)

add_definitions (-D__clang_analyzer__ -D_CRT_SECURE_NO_WARNINGS)

add_definitions (-DUNICODE -D_UNICODE)

project ("Core")

add_library (${PROJECT_NAME} STATIC
    "Types.hpp"
	"Memory/MemoryManager.hpp"
	"Memory/MemoryManager.cpp"
	"Memory/Memory.hpp"
	"Math/MathUtils.hpp"
)

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
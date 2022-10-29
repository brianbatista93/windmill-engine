project ("Core")

add_executable (${PROJECT_NAME}
    "Types.hpp"
	"Memory/MemoryManager.hpp"
	"Memory/MemoryManager.cpp"
	"Math/MathUtils.hpp"
)

target_include_directories (${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
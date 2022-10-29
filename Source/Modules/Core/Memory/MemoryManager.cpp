#include <malloc.h>

#include "MemoryManager.hpp"
#include "Math/MathUtils.hpp"

CMemoryManager& CMemoryManager::Get() {
  static CMemoryManager sInstance;
  return sInstance;
}

CMemoryManager::~CMemoryManager() {
  for (const auto& [address, desc] : m_AllocatedMemory) {
    printf("Memory leak detected: %s(%d): %s\n", desc.m_pFilename, desc.m_nLine, desc.m_pFunctionName);
    printf("Memory address: %p\n", desc.m_pMemory);
    printf("Memory size: %zu\n", desc.m_nSize);
    printf("Memory alignment: %zu\n", desc.m_nAlignment);
  }
}

void* CMemoryManager::Reallocate(
    void* pMemory, size_t nSize, size_t nAlignment, const char* pFilename, i32 nLine, const char* pFunctionName) {

  we_assert(nSize > 0 && "Size must be greater than 0.");
  we_assert(CMath::IsPowerOfTwo(nAlignment) && "Alignment must be a power of 2.");

  CMemoryAllocationDesc desc = {
      .m_pMemory       = pMemory,
      .m_nSize         = nSize,
      .m_nAlignment    = nAlignment,
      .m_pFilename     = pFilename,
      .m_nLine         = nLine,
      .m_pFunctionName = pFunctionName};

  return ReallocateInternal(&desc);
}

void CMemoryManager::Free(void* pMemory) {
  if (pMemory) {
    FreeInternal((size_t)pMemory);
  }
}

void* CMemoryManager::ReallocateInternal([[maybe_unused]] const CMemoryAllocationDesc* desc) {
  we_assert(desc != nullptr && "Allocation descriptor must not be null.");

  void* memory    = realloc(desc->m_pMemory, desc->m_nSize);
  desc->m_pMemory = memory;

  if (memory == nullptr) {
    return nullptr;
  }

  m_AllocatedMemory.emplace(std::make_pair((size_t)memory, *desc));

  return memory;
}

void CMemoryManager::FreeInternal(size_t memoryAddress) {
  auto it = m_AllocatedMemory.find(memoryAddress);

  we_assert(it != m_AllocatedMemory.end() && "Memory address must be allocated.");

  const CMemoryAllocationDesc* desc = &it->second;

  free(desc->m_pMemory);

  m_AllocatedMemory.erase(it);
}

int main() {
  i32* p = (i32*)we_calloc(sizeof(i32));
  *p     = 29;

  // we_free(p);

  return 0;
}
#include <malloc.h>

#include "MemoryManager.hpp"
#include "Math/MathUtils.hpp"

namespace WE::Internal {
inline size_t
AlignAddress(size_t nAddress, size_t nAlign)
{
  const size_t mask = nAlign - 1;
  return (nAddress + mask) & ~mask;
}

inline u8*
AlignPointer(u8* pAddress, size_t nAlign)
{
  const size_t address = reinterpret_cast<size_t>(pAddress);
  return reinterpret_cast<u8*>(AlignAddress(address, nAlign));
}
} // namespace WE::Internal

CMemoryManager&
CMemoryManager::Get()
{
  static CMemoryManager sInstance;
  return sInstance;
}

u32 CMemoryManager::sLastAllocationID = 1;

CMemoryManager::~CMemoryManager()
{
  for (const auto& [address, desc] : m_AllocatedMemory) {
    printf("Memory leak detected: %s(%d): %s\n", desc.m_pFilename, desc.m_nLine, desc.m_pFunctionName);
    printf("Memory address: %p\n", desc.m_pMemory);
    printf("Memory size: %zu\n", desc.m_nSize);
    printf("Memory alignment: %zu\n", desc.m_nAlignment);
  }
}

void*
CMemoryManager::Reallocate(
  void* pMemory, size_t nSize, size_t nAlignment, const char* pFilename, i32 nLine, const char* pFunctionName)
{

  we_assert(nSize > 0 && "Size must be greater than 0.");
  we_assert(CMath::IsPowerOfTwo(nAlignment) && "Alignment must be a power of 2.");

  CMemoryAllocationDesc desc = {
    .m_pMemory       = pMemory,
    .m_nSize         = nSize,
    .m_nAlignment    = nAlignment,
    .m_pFilename     = pFilename,
    .m_pFunctionName = pFunctionName,
    .m_nLine         = nLine,
    .m_nID           = sLastAllocationID++
  };

  return ReallocateInternal(&desc);
}

void
CMemoryManager::Free(void* pMemory)
{
  if (pMemory) {
    FreeInternal((size_t)pMemory);
  }
}

void*
CMemoryManager::ReallocateInternal(const CMemoryAllocationDesc* pDesc)
{
  we_assert(pDesc != nullptr && "Allocation descriptor must not be null.");

  size_t realSize = pDesc->m_nSize + pDesc->m_nAlignment;

  u8* rawMemory       = (u8*)realloc(pDesc->m_pRawMemory, realSize);
  pDesc->m_pRawMemory = rawMemory;

  u8* alignedMemory = WE::Internal::AlignPointer(rawMemory, pDesc->m_nAlignment);
  if (alignedMemory == rawMemory) {
    alignedMemory += pDesc->m_nAlignment;
  }

  m_AllocatedMemory.emplace(std::make_pair((size_t)alignedMemory, *pDesc));
  pDesc->m_pMemory = alignedMemory;

  return alignedMemory;
}

void
CMemoryManager::FreeInternal(size_t nMemoryAddress)
{
  auto it = m_AllocatedMemory.find(nMemoryAddress);

  we_assert(it != m_AllocatedMemory.end() && "Memory address must be allocated.");

  const CMemoryAllocationDesc* desc = &it->second;

  free(desc->m_pRawMemory);

  m_AllocatedMemory.erase(it);
}
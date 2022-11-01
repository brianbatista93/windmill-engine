#include <malloc.h>

#include "Math/MathUtils.hpp"
#include "MemoryManager.hpp"

namespace WE::Internal
{
inline void CheckAllocationInput(size_t nSize, size_t nAlignment)
{
    we_assert(nSize > 0 && "Size must be greater than 0.");
    we_assert(CMath::IsPowerOfTwo(nAlignment) && "Alignment must be a power of 2.");
}
} // namespace WE::Internal

CMemoryManager &CMemoryManager::Get()
{
    static CMemoryManager sInstance;
    return sInstance;
}

CMemoryManager::~CMemoryManager()
{
    // we_assert(m_CurrentAllocations.empty() && "There are still allocations that have not been freed.");
    for (auto [memory, info] : m_CurrentAllocations)
    {
        printf("0x%08llx (%zx|%d) - %s:%d (%s)\n", size_t(memory), info.nSize, info.nAlignment, info.pFilename, info.nLine,
               info.pFunctionName);
    }
}

static i32 lastOrder = 1;

void *CMemoryManager::Allocate(size_t nSize, size_t nAlignment, const char *pFilename, i32 nLine,
                               const char *pFunctionName)
{
    WE::Internal::CheckAllocationInput(nSize, nAlignment);

    SAllocationInfo info = {};
    info.pFilename = pFilename;
    info.pFunctionName = pFunctionName;
    info.nSize = nSize;
    info.nLine = nLine;
    info.nAlignment = i32(nAlignment);
    info.nOrder = lastOrder++;

    void *pointer = MallocInternal(nSize, nAlignment);

    AddAllocationInfo(pointer, &info);

    return pointer;
}

void *CMemoryManager::Reallocate(void *pMemory, size_t nSize, size_t nAlignment, const char *pFilename, i32 nLine,
                                 const char *pFunctionName)
{
    WE::Internal::CheckAllocationInput(nSize, nAlignment);

    SAllocationInfo info = {};
    info.pFilename = pFilename;
    info.pFunctionName = pFunctionName;
    info.nSize = nSize;
    info.nLine = nLine;
    info.nAlignment = i32(nAlignment);
    info.nOrder = lastOrder++;

    void *newPointer = ReallocInternal(pMemory, nSize, nAlignment);

    EditAllocationInfo(pMemory, newPointer, &info);

    return newPointer;
}

void CMemoryManager::Free(void *pMemory)
{
    if (pMemory != nullptr)
    {
        RemoveAllocationInfo(pMemory);
    }
}

void *CMemoryManager::MallocInternal(size_t nSize, size_t nAlignment)
{
#if defined(WE_OS_WINDOWS)
    return _aligned_malloc(nSize, nAlignment);
#else
    return malloc(nSize);
#endif // defined(WE_OS_WINDOWS)
}

void *CMemoryManager::ReallocInternal(void *pMemory, size_t nSize, size_t nAlignment)
{
#if defined(WE_OS_WINDOWS)
    return _aligned_realloc(pMemory, nSize, nAlignment);
#else
    return realloc(nSize);
#endif // defined(WE_OS_WINDOWS)
}

void CMemoryManager::FreeInternal(void *pMemory)
{
#if defined(WE_OS_WINDOWS)
    return _aligned_free(pMemory);
#else
    return free(pMemory);
#endif // defined(WE_OS_WINDOWS)
}

void CMemoryManager::AddAllocationInfo(void *pMemory, const SAllocationInfo *pInfo)
{
    m_CurrentAllocations.emplace(std::make_pair(pMemory, *pInfo));
}

void CMemoryManager::EditAllocationInfo(void *pOldMemory, void *pNewMemory, const SAllocationInfo *pInfo)
{
    if (pOldMemory != nullptr)
    {
        RemoveAllocationInfo(pOldMemory);
    }

    AddAllocationInfo(pNewMemory, pInfo);
}

void CMemoryManager::RemoveAllocationInfo(void *pMemory)
{
    m_CurrentAllocations.erase(pMemory);
}
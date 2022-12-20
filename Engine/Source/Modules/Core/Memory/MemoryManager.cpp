#include <cstring>
#include <iomanip>
#include <iostream>
#include <locale>
#include <malloc.h>

#include "Math/MathUtils.hpp"
#include "MemoryManager.hpp"
#include "OS/Utils.hpp"

namespace WE::Internal
{
inline void CheckAllocationInput([[maybe_unused]] usize nSize, [[maybe_unused]] usize nAlignment)
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
    fprintf(stderr, "\n[INFO] Memory manager have exited with [%d] unfreed allocations.\n", (i32)m_CurrentAllocations.size());

    if (!m_CurrentAllocations.empty())
    {
        std::cerr << std::setfill('=') << std::setw(120) << '\n';
        u32 unfreedBytes = 0;
        fprintf(stderr, "[ERROR] There are still allocations that have not been freed.\n");
        for (auto [memory, info] : m_CurrentAllocations)
        {
            fprintf(stderr, "0x%08llux (%d bytes|%d) - %s:%d (%s)\n", u64(memory), i32(info.nSize), info.nAlignment, info.pFilename, info.nLine,
                    info.pFunctionName);
#if WE_OS_SUPPORT_CALLSTACK_INFO
            for (u32 i = 0; i < info.nCallStackFrames; ++i)
            {
                fprintf(stderr, "\t> %s\n", info.ppCallStack[i]);
            }
#endif // WE_OS_SUPPORT_CALLSTACK_INFO

            unfreedBytes += (u32)info.nSize;
        }

        std::cerr << std::setfill('=') << std::setw(120) << '\n';

        std::cerr.imbue(std::locale(""));
        std::cerr << "[ERROR] Total unfreed memory: " << i32(unfreedBytes << 20) << " (bytes)" << std::endl;
    }
}

static i32 lastOrder = 1;

void *CMemoryManager::Allocate(usize nSize, usize nAlignment, const char *pFilename, i32 nLine, const char *pFunctionName)
{
    WE::Internal::CheckAllocationInput(nSize, nAlignment);

    SAllocationInfo info = {};
    info.pFilename = pFilename;
    info.pFunctionName = pFunctionName;
    info.nSize = nSize;
    info.nLine = nLine;
    info.nAlignment = i32(nAlignment);
    info.nOrder = lastOrder++;

#if WE_OS_SUPPORT_CALLSTACK_INFO
    info.nCallStackFrames = OS::GetStackTrace(2, 32, info.ppCallStack, 255, info.ppCallers);
#endif // WE_OS_SUPPORT_CALLSTACK_INFO

    void *pointer = MallocInternal(nSize, nAlignment);

    AddAllocationInfo(pointer, &info);

    return pointer;
}

void *CMemoryManager::Reallocate(void *pMemory, usize nSize, usize nAlignment, const char *pFilename, i32 nLine, const char *pFunctionName)
{
    WE::Internal::CheckAllocationInput(nSize, nAlignment);

    SAllocationInfo info = {};
    info.pFilename = pFilename;
    info.pFunctionName = pFunctionName;
    info.nSize = nSize;
    info.nLine = nLine;
    info.nAlignment = i32(nAlignment);
    info.nOrder = lastOrder++;

#if WE_OS_SUPPORT_CALLSTACK_INFO
    info.nCallStackFrames = OS::GetStackTrace(2, 32, info.ppCallStack, 255, info.ppCallers);
#endif // WE_OS_SUPPORT_CALLSTACK_INFO

    void *newPointer = ReallocInternal(pMemory, nSize, nAlignment);

    EditAllocationInfo(pMemory, newPointer, &info);

    return newPointer;
}

void CMemoryManager::Free(void *pMemory)
{
    if (pMemory != nullptr)
    {
        FreeInternal(pMemory);
        RemoveAllocationInfo(pMemory);
    }
}

void *CMemoryManager::MallocInternal(usize nSize, [[maybe_unused]] usize nAlignment)
{
#if defined(WE_OS_WINDOWS) && defined(WE_OS_ALIGNED_ALLOCATION)
    return _aligned_malloc(nSize, nAlignment);
#else
    return malloc(nSize);
#endif // defined(WE_OS_WINDOWS)
}

void *CMemoryManager::ReallocInternal(void *pMemory, usize nSize, [[maybe_unused]] usize nAlignment)
{
    if (pMemory == nullptr)
    {
        return MallocInternal(nSize, nAlignment);
    }

#if defined(WE_OS_WINDOWS) && defined(WE_OS_ALIGNED_ALLOCATION)
    return _aligned_realloc(pMemory, nSize, nAlignment);
#else
    return realloc(pMemory, nSize);
#endif // defined(WE_OS_WINDOWS)
}

void CMemoryManager::FreeInternal(void *pMemory)
{
#if defined(WE_OS_WINDOWS) && defined(WE_OS_ALIGNED_ALLOCATION)
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
    [[maybe_unused]] usize erased = m_CurrentAllocations.erase(pMemory);
    we_assert(erased && "Trying to free an unallocated memory.");
}

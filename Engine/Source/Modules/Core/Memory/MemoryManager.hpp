/*
Copyright (C) 2021-2022 Bull Technology, Ltd.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//--------------------------------------------------------------------------------------------
// NOTE: Please avoid including this header file, use Memory.hpp instead
//--------------------------------------------------------------------------------------------

#pragma once

#include <map>
#include <utility>

#include "OS/Utils.hpp"
#include "Types.hpp"

/**
 * @brief Manager responsible for allocating, freeing and managing memory.
 */
class CMemoryManager
{
  public:
    static CMemoryManager &Get();

    // NOLINTNEXTLINE
    ~CMemoryManager();

    void *Allocate(usize nSize, usize nAlign, const char *pFilename, i32 nLine, const char *pFunctionName);
    void *Reallocate(void *pMemory, usize nSize, usize nAlign, const char *pFilename, i32 nLine, const char *pFunctionName);
    void Free(void *pMemory);

    template <class T, class... Args>
    static T *New(const char *pFilename, i32 nLine, const char *pFunctionName, Args &&...args)
    {
        T *ptr = (T *)Get().Allocate(sizeof(T), alignof(T), pFilename, nLine, pFunctionName);
        return new (ptr) T(std::forward<Args>(args)...);
    }

    template <class T>
    static void Delete(T *ptr)
    {
        if (ptr)
        {
            ptr->~T();
            Get().Free(ptr);
        }
    }

  private:
    struct SAllocationInfo
    {
        const char *pFilename;
        const char *pFunctionName;
        usize nSize;
        i32 nLine;
        i32 nAlignment;
        i32 nOrder;

#ifdef WE_OS_SUPPORT_CALLSTACK_INFO
        ansi ppCallStack[MAX_CALLSTACKS][MAX_SIMBOLS_LENGTH];
        u32 nCallStackFrames;
        void *ppCallers[MAX_CALLSTACKS];
#endif // WE_OS_SUPPORT_CALLSTACK_INFO
    };

    std::map<usize, SAllocationInfo> mCurrentAllocations;

    CMemoryManager() = default;

    static void *MallocInternal(usize nSize, usize nAlignment);

    void *ReallocInternal(void *pMemory, usize nSize, usize nAlignment);

    static void FreeInternal(void *pMemory);

    void AddAllocationInfo(usize nMemoryAddress, const SAllocationInfo *pInfo);

    void EditAllocationInfo(usize nOldMemoryAddress, void *pNewMemory, const SAllocationInfo *pInfo);

    void RemoveAllocationInfo(usize nMemoryAddress);
};

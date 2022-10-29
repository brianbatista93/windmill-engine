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

#include "Types.hpp"

/**
 * @brief Manager responsible for allocating, freeing and managing memory.
 */
class CMemoryManager
{
public:
  static CMemoryManager& Get();

  ~CMemoryManager();

  void* Reallocate(
    void* pMemory, size_t nSize, size_t nAlign, const char* pFilename, i32 nLine, const char* pFunctionName);
  void Free(void* pMemory);

  template<class T, class... Args>
  static T* New(const char* pFilename, i32 nLine, const char* pFunctionName, Args&&... args)
  {
    T* ptr = (T*)Reallocate(nullptr, sizeof(T), alignof(T), pFilename, nLine, pFunctionName);
    return new (ptr) T(std::forward<Args>(args)...);
  }

  template<class T>
  static void Delete(T* ptr, const char* pFilename, i32 nLine, const char* pFunctionName)
  {
    if (ptr) {
      ptr->~T();
      Free(ptr);
    }
  }

private:
  struct CMemoryAllocationDesc
  {
    mutable void* m_pMemory;
    mutable void* m_pRawMemory;
    size_t m_nSize;
    size_t m_nAlignment;
    const char* m_pFilename;
    const char* m_pFunctionName;
    i32 m_nLine;
    u32 m_nID;
  };

  void* ReallocateInternal(const CMemoryAllocationDesc* pDesc);

  void FreeInternal(size_t nMemoryAddress);

  std::map<size_t, CMemoryAllocationDesc> m_AllocatedMemory;

  static u32 sLastAllocationID;
};

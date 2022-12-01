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

#pragma once

#include "MemoryManager.hpp"
#include "MemoryUtils.hpp"

#ifndef we_malloc
    #define we_malloc(size)                                                                                            \
        CMemoryManager::Get().Allocate((size), WE_OS_MEM_DEFAULT_ALIGNMENT, __FILE__, __LINE__, __FUNCTION__)
#endif // !we_malloc

#ifndef we_calloc
    #define we_calloc(count, size)                                                                                     \
        CMemoryManager::Get().Allocate((count) * (size), WE_OS_MEM_DEFAULT_ALIGNMENT, __FILE__, __LINE__, __FUNCTION__)
#endif // !we_calloc

#ifndef we_realloc
    #define we_realloc(data, size)                                                                                     \
        CMemoryManager::Get().Reallocate((data), (size), WE_OS_MEM_DEFAULT_ALIGNMENT, __FILE__, __LINE__, __FUNCTION__)
#endif // !we_realloc

#ifndef we_free
    #define we_free(mem) CMemoryManager::Get().Free((mem))
#endif // !we_free

#ifndef we_new
    #define we_new(type, ...) CMemoryManager::Get().New<type>(__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif // !we_new

#ifndef we_delete
    #define we_delete(ptr) CMemoryManager::Get().Delete(ptr, __FILE__, __LINE__, __FUNCTION__)
#endif // !we_delete

#ifndef MEMORY_MANAGER_FROM_HEADER
    #ifndef malloc
        #define malloc(size) static_assert(false, "Please use we_malloc");
    #endif
    #ifndef calloc
        #define calloc(count, size) static_assert(false, "Please use we_calloc");
    #endif
    #ifndef realloc
        #define realloc(ptr, size) static_assert(false, "Please use we_realloc");
    #endif
    #ifndef free
        #define free(ptr) static_assert(false, "Please use we_free");
    #endif

    #ifndef new
        #define new static_assert(false, "Please use we_placement_new");
    #endif
    #ifndef delete
        #define delete static_assert(false, "Please use we_free with explicit destructor call");
    #endif

#endif //! MEMORY_MANAGER_FROM_HEADER

#ifdef MEMORY_MANAGER_FROM_HEADER
    #undef MEMORY_MANAGER_FROM_HEADER
#endif // MEMORY_MANAGER_FROM_HEADER

#include <memory>

template <class T>
class TWEDeleter
{
  public:
    constexpr void operator()(T *pObject) const noexcept { we_delete(pObject); }
};

template<class T>
using TUniquePtr = std::unique_ptr<T, TWEDeleter<T>>;
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

#define MEMORY_MANAGER_FROM_HEADER
#include "Memory/Memory.hpp"

#include "Types.hpp"

template <class TIndexType>
class CAllocator
{
  public:
    using IndexType = TIndexType;

    ~CAllocator() { we_assert(mData == nullptr && "The allocator is been destroyed without releasing its data"); }

    void *Reallocate(usize nSize)
    {
        if (mData != nullptr or nSize != 0)
        {
            mData = we_realloc(mData, nSize);
        }

        return mData;
    }

    NDISCARD void *GetData() const { return mData; }

    void MoveTo(CAllocator &rOther) noexcept
    {
        rOther.mData = mData;
        mData = nullptr;
    }

    void ReleaseData()
    {
        we_free(mData);
        mData = nullptr;
    }

  private:
    void *mData{nullptr};
};

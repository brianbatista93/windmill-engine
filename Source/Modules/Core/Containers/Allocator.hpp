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

template <class TIndexType> class TAllocator
{
  public:
    using IndexType = TIndexType;

    ~TAllocator() { ReleaseData(); }

    void *Reallocate(usize nSize)
    {
        if (m_pData != nullptr or nSize != 0)
        {
            m_pData = we_realloc(m_pData, nSize);
        }

        return m_pData;
    }

    void *GetData() const { return m_pData; }

    void MoveTo(TAllocator &rOther) noexcept
    {
        rOther.m_pData = m_pData;
        m_pData = nullptr;
    }

    void ReleaseData()
    {
        we_free(m_pData);
        m_pData = nullptr;
    }

  private:
    void *m_pData{nullptr};
};
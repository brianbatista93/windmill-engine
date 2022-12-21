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

#include "Array.hpp"

template <class T, class Alloc>
class TArrayView
{
  public:
    using SizeType = typename Alloc::IndexType;

    TArrayView() : m_pBegin(nullptr), m_pEnd(nullptr) {}
    TArrayView(TArray<T, Alloc> &arr) : m_pBegin(arr.begin()), m_pEnd(arr.end()) {}
    ~TArrayView()
    {
        m_pBegin = nullptr;
        m_pEnd = nullptr;
    }

    TArrayView(TArrayView &&) = default;
    TArrayView(const TArrayView &) = default;
    TArrayView &operator=(TArrayView &&) = default;
    TArrayView &operator=(const TArrayView &) = default;

    constexpr bool IsEmpty() const { return m_pEnd == m_pBegin; }

  private:
    T *m_pBegin;
    T *m_pEnd;
};

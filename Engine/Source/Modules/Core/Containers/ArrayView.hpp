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

template <class Type, class AllocType>
class CArrayView
{
  public:
    using SizeType = typename AllocType::IndexType;

    CArrayView() : mBegin(nullptr), mEnd(nullptr) {}
    CArrayView(CArray<Type, AllocType> &arr) : mBegin(arr.begin()), mEnd(arr.end()) {}
    ~CArrayView()
    {
        mBegin = nullptr;
        mEnd = nullptr;
    }

    CArrayView(CArrayView &&) noexcept = default;
    CArrayView(const CArrayView &) = default;
    CArrayView &operator=(CArrayView &&) noexcept = default;
    CArrayView &operator=(const CArrayView &) = default;

    NDISCARD constexpr bool IsEmpty() const { return mEnd == mBegin; }

  private:
    Type *mBegin;
    Type *mEnd;
};

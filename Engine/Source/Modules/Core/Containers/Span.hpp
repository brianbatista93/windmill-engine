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

#include <span>

static constexpr usize DynamicExtent = usize{~0u};

template <class Type, usize Extent = DynamicExtent>
class CSpan
{
  public:
    CSpan() noexcept { static_assert(Extent == DynamicExtent || Extent == 0, "Extent must be 0 for default constructor"); }

    CSpan(Type *pBegin, usize nCount) : mData{pBegin}, mSize{nCount} { we_assert(Extent == DynamicExtent || Extent == nCount); }

    CSpan(Type *pBegin, Type *pEnd) : mData{pBegin}, mSize{static_cast<usize>(pEnd - pBegin)} {}

    template <typename UType, usize N>
        requires((Extent == DynamicExtent || Extent == N) && std::convertible_to<UType (*)[], Type (*)[]>)
    CSpan(const CSpan<UType, N> &other) noexcept : CSpan{other.mData, other.mSize}
    {
    }

    ~CSpan() noexcept = default;
    CSpan(const CSpan &) noexcept = default;
    CSpan &operator=(const CSpan &) noexcept = default;

  private:
    Type *mData{nullptr};
    usize mSize{0};
};

#ifdef __cpp_deduction_guides
template <class Type, size_t N>
CSpan(Type (&)[N]) -> CSpan<Type, N>;
#endif

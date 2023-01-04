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

#include "Containers/StringUtils.hpp"
#include "Math/MathUtils.hpp"
#include "WeString.hpp"

class CStringView
{
  public:
    CStringView() = default;
    CStringView(const CStringView &) = default;
    CStringView(CStringView &&) noexcept = default;
    CStringView &operator=(const CStringView &) = default;
    CStringView &operator=(CStringView &&) noexcept = default;

    inline CStringView(CString &str) : mBegin(str.begin()), mEnd(str.end())
    {
#ifdef WE_DEBUG
        memcpy(mDataViewer, mBegin, CMath::Min<usize>(255, mEnd - mBegin) * sizeof(tchar));
#endif // WE_DEBUG
    }

    inline CStringView(const tchar *begin) : mBegin(const_cast<tchar *>(begin))
    {
        const usize length = std::char_traits<tchar>::length(begin);
        mEnd = mBegin + length - 1;
    }

    inline CStringView(tchar *begin, tchar *end) : mBegin(begin), mEnd(end)
    {
#ifdef WE_DEBUG
        memcpy(mDataViewer, mBegin, CMath::Min<usize>(255, mEnd - mBegin) * sizeof(tchar));
#endif // WE_DEBUG
    }

    inline bool IsEmpty() const { return mEnd == mBegin; }

    inline i32 GetLength() const { return static_cast<i32>(mEnd - mBegin); }

    inline i32 Find(tchar chr, i32 nOffset = 0) const { return CStringUtils::Find(mBegin, GetLength(), chr, nOffset); }

    inline void Remove(i32 nStart, i32 nCount = -1)
    {
        if (nStart != -1)
        {
            we_assert((mBegin + nStart) <= mEnd);

            mBegin += nStart;
            nCount = static_cast<i32>(mEnd - mBegin);
            mBegin += nCount;

#ifdef WE_DEBUG
            memcpy(mDataViewer, mBegin, CMath::Min<usize>(255, mEnd - mBegin) * sizeof(tchar));
#endif // WE_DEBUG
        }
    }

    inline CStringView Substring(i32 nStart, i32 nCount = -1) const
    {
        we_assert((mBegin + nStart) <= mEnd);
        nCount = nCount == -1 ? static_cast<i32>(mEnd - mBegin) : nCount;
        return {mBegin + nStart, mBegin + nStart + nCount};
    }

    inline CString ToString() const
    {
        if (!IsEmpty())
        {
            return {mBegin, mEnd + 1};
        }

        return {};
    }

    inline tchar &operator[](i32 nIndex) { return mBegin[nIndex]; }
    inline tchar operator[](i32 nIndex) const { return mBegin[nIndex]; }

    inline tchar *begin() { return mBegin; }
    inline tchar *end() { return mEnd; }

    inline const tchar *begin() const { return mBegin; }
    inline const tchar *end() const { return mEnd; }

  private:
    mutable tchar *mBegin{nullptr};
    mutable tchar *mEnd{nullptr};

#ifdef WE_DEBUG
    tchar mDataViewer[256]{0};
#endif // WE_DEBUG
};

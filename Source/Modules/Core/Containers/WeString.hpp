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

#include "Allocator.hpp"
#include "Array.hpp"
#include "Concepts.hpp"
#include "StringBuilder.hpp"
#include "StringUtils.hpp"

#include <string>

class CString
{
  public:
    using CharType = tchar;
    using ArrayType = TArray<CharType, TAllocator<i32>>;

    enum
    {
        INVALID_INDEX = -1
    };

    CString() = default;
    CString(CString &&) = default;
    CString(const CString &) = default;
    CString &operator=(CString &&) = default;
    CString &operator=(const CString &) = default;

    CString(std::nullptr_t) = delete;

    inline CString(const tchar *pStr, usize nLength) : m_Data(i32(nLength) + 1) { Init(pStr, m_Data.GetSize()); }

    CString(const u8 *pBytes, usize nSize, const class IEncoder *pEncoder);

    inline CString &Append(tchar chr)
    {
        const i32 index = m_Data.AddSlots() - 1;
        m_Data[index] = chr;
        return *this;
    }

    inline CString &Append(const tchar *pStr)
    {
        const usize length = std::char_traits<tchar>::length(pStr);
        return Append(pStr, i32(length));
    }

    inline CString &Append(const tchar *pStr, i32 nLength)
    {
        const i32 index = m_Data.AddSlots(nLength) - 1;
        memcpy(m_Data.GetData() + index, pStr, nLength * sizeof(tchar));
        return *this;
    }

    inline CString operator+(tchar chr) const { return CString(*this).Append(chr); }

    inline CString &operator+=(tchar chr) { return Append(chr); }
    inline CString &operator+=(const tchar *pStr) { return Append(pStr); }

    template <class TStr>
    inline CString &operator+=(TStr &&rhs) noexcept
    {
        return Append(std::forward<TStr>(rhs));
    }

    friend inline CString operator+(CString &&lhs, CString &&rhs) noexcept { return ConcatSS(std::move(lhs), std::move(rhs)); }
    friend inline CString operator+(CString &lhs, const CString &&rhs) noexcept { return ConcatSS(std::move(lhs), rhs); }
    friend inline CString operator+(const CString &lhs, CString &&rhs) noexcept { return ConcatSS(lhs, std::move(rhs)); }
    friend inline CString operator+(const CString &lhs, const CString &rhs) { return ConcatSS(lhs, rhs); }

    friend inline CString operator+(CString &&lhs, const tchar *pRhs) noexcept { return ConcatSC(std::move(lhs), pRhs); }
    friend inline CString operator+(const CString &lhs, const tchar *pRhs) { return ConcatSC(lhs, pRhs); }

    friend inline CString operator+(const tchar *pLhs, CString &&rhs) noexcept { return ConcatCS(pLhs, std::move(rhs)); }
    friend inline CString operator+(const tchar *pLhs, const CString &rhs) { return ConcatCS(pLhs, rhs); }

    inline i32 GetLength() const { return m_Data.GetSize() > 0 ? (m_Data.GetSize() - 1) : 0; }
    inline bool IsEmpty() const { return GetLength() == 0; }

    inline const CharType *operator*() const { return IsEmpty() ? nullptr : m_Data.GetData(); }

    inline ArrayType GetArray() const { return m_Data; }

    inline bool StartsWith(const tchar *pStart, i32 nOffset = 0)
    {
        we_assert(nOffset < GetLength());
        return CStringUtils::StartsWith(GetArray().GetData() + nOffset, pStart);
    }

    inline bool EndsWith(const tchar *pEnd, i32 nOffset = 0)
    {
        we_assert(nOffset < GetLength());
        return CStringUtils::EndsWith(GetArray().GetData() + nOffset, pEnd, GetLength());
    }

    inline CString Substring(i32 nStart, i32 nCount = INVALID_INDEX)
    {
        we_assert(nStart < GetLength() and (nCount == INVALID_INDEX or nCount <= GetLength()));
        nCount = nCount == INVALID_INDEX ? GetLength() : nCount;
        return CString(GetArray().GetData() + nStart, nCount);
    }

    inline i32 Find(const tchar *pStr, i32 nOffset = 0)
    {
        we_assert(pStr != nullptr and nOffset < GetLength());
        return CStringUtils::Find(GetArray().GetData(), pStr, nOffset);
    }

    TArray<CString, TAllocator<i32>> Split(const tchar *pStr);

    template <WE::Concept::IsContainer TContainer>
    inline static CString Join(tchar separator, const TContainer &container)
    {
        CStringBuilder builder;
        bool bIsFirst = true;

        for (CString str : container)
        {
            if (!bIsFirst) [[unlikely]]
            {
                builder.Append(separator);
            }
            builder.Append(str);
            bIsFirst = false;
        }

        return builder.ToString();
    }

    template <WE::Concept::IsContainer TContainer>
    inline static CString Join(const tchar *separator, const TContainer &container)
    {
        CStringBuilder builder;
        bool bIsFirst = true;

        for (CString str : container)
        {
            if (!bIsFirst) [[unlikely]]
            {
                builder.Append(separator);
            }
            builder.Append(str);
            bIsFirst = false;
        }

        return builder.ToString();
    }

    template <class... TArgs>
    inline static CString Format(const tchar *pFormat, TArgs &&...args)
    {
        CStringBuilder builder(256);
        builder.AppendFormat(pFormat, std::forward<TArgs>(args)...);
        return builder.ToString();
    }

    inline bool operator==(const CString &other) const { return CStringUtils::Compare(GetArray().GetData(), *other) == 0; }

  private:
    inline void Init(const tchar *pStr, usize nLength)
    {
        if (nLength > 0)
        {
            memcpy(m_Data.GetData(), pStr, nLength * sizeof(CharType));
            m_Data[i32(nLength) - 1] = tchar(0);
        }
    }

    static CString ConcatSS(CString &&lhs, CString &&rhs) noexcept;
    static CString ConcatSS(CString &&lhs, const CString &rhs) noexcept;
    static CString ConcatSS(const CString &lhs, CString &&rhs) noexcept;
    static CString ConcatSS(const CString &lhs, const CString &rhs);

    static CString ConcatSC(CString &&lhs, const tchar *pRhs) noexcept;
    static CString ConcatSC(const CString &lhs, const tchar *pRhs);

    static CString ConcatCS(const tchar *pLhs, CString &&rhs) noexcept;
    static CString ConcatCS(const tchar *pLhs, const CString &rhs);

    ArrayType m_Data;
};

inline namespace Literals
{
inline namespace StringLiterals
{
inline CString operator""_s(const tchar *pStr, usize nLength)
{
    return CString(pStr, nLength);
}
} // namespace StringLiterals
} // namespace Literals

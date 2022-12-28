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
#include "ContainerFwd.hpp"
#include "StringBuilder.hpp"
#include "StringUtils.hpp"

#include <string>

class CString
{
    friend class CPath;

  public:
    using CharType = tchar;
    using ArrayType = CArray<CharType, DefaultAllocator>;

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

    inline explicit CString(const tchar *pStr) : mData(i32(std::char_traits<tchar>::length(pStr)) + 1) { Init(pStr, mData.GetSize()); }

    inline CString(const tchar *pStr, usize nLength) : mData(i32(nLength) + 1) { Init(pStr, mData.GetSize()); }

    CString(const u8 *pBytes, usize nSize, const class IEncoder *pEncoder);

    template <class ItType>
    CString(ItType *begin, ItType *end) : mData(i32(end - begin))
    {
        Init(begin, mData.GetSize());
    }

    inline CString &Append(tchar chr)
    {
        const i32 index = mData.AddSlots() - 1;
        mData[index] = chr;
        return *this;
    }

    inline CString &Append(const tchar *pStr)
    {
        const usize length = std::char_traits<tchar>::length(pStr);
        return Append(pStr, i32(length));
    }

    inline CString &Append(const tchar *pStr, i32 nLength)
    {
        const i32 index = mData.AddSlots(nLength) - 1;
        memcpy(mData.GetData() + index, pStr, nLength * sizeof(tchar));
        return *this;
    }

    inline CString operator+(tchar chr) const { return CString(*this).Append(chr); }

    inline CString &operator+=(tchar chr) { return Append(chr); }
    inline CString &operator+=(const tchar *pStr) { return Append(pStr); }

    template <class TStrType>
    inline CString &operator+=(TStrType &&rhs) noexcept
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

    NDISCARD inline i32 GetLength() const { return mData.GetSize() > 0 ? (mData.GetSize() - 1) : 0; }
    NDISCARD inline bool IsEmpty() const { return GetLength() == 0; }

    inline const CharType *operator*() const { return IsEmpty() ? WT("") : mData.GetData(); }

    inline ArrayType &GetArray() { return mData; }
    NDISCARD inline ArrayType GetArray() const { return mData; }

    inline bool StartsWith(const tchar *pStart, i32 nOffset = 0) const
    {
        we_assert(nOffset < GetLength());
        return CStringUtils::StartsWith(GetArray().GetData() + nOffset, pStart);
    }

    inline bool EndsWith(const tchar *pEnd, i32 nOffset = 0) const
    {
        we_assert(nOffset < GetLength());
        return CStringUtils::EndsWith(GetArray().GetData() + nOffset, pEnd, GetLength());
    }

    NDISCARD inline CString Substring(i32 nStart, i32 nCount = INVALID_INDEX) const
    {
        we_assert(nStart < GetLength() and (nCount == INVALID_INDEX or nCount <= GetLength()));
        nCount = nCount == INVALID_INDEX ? GetLength() : nCount;
        return {GetArray().GetData() + nStart, (usize)nCount};
    }

    inline i32 Find(const tchar *pStr, i32 nOffset = 0) const
    {
        we_assert(pStr != nullptr and nOffset < GetLength());
        return CStringUtils::Find(GetArray().GetData(), pStr, nOffset);
    }

    NDISCARD CArray<CString> Split(tchar chr) const;
    CArray<CString> Split(const tchar *pStr) const;

    template <WE::Concept::IsContainer TContainerType>
    inline static CString Join(tchar separator, const TContainerType &container)
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

    template <WE::Concept::IsContainer TContainerType>
    inline static CString Join(const tchar *separator, const TContainerType &container)
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

    template <class... ArgsType>
    inline static CString Format(const tchar *pFormat, ArgsType &&...packedArgs)
    {
        CStringBuilder builder(256);
        builder.AppendFormat(pFormat, std::forward<ArgsType>(packedArgs)...);
        return builder.ToString();
    }

    inline bool operator==(const CString &other) const { return CStringUtils::Compare(GetArray().GetData(), *other) == 0; }

    NDISCARD auto begin() { return mData.begin(); }
    NDISCARD auto begin() const { return mData.begin(); }
    NDISCARD auto end()
    {
        auto *result = mData.end();
        if (mData.GetSize())
        {
            --result;
        }
        return result;
    }
    NDISCARD auto end() const
    {
        const auto *result = mData.end();
        if (mData.GetSize())
        {
            --result;
        }
        return result;
    }

    NDISCARD auto rbegin() { return std::reverse_iterator(mData.end()); }
    NDISCARD auto rbegin() const { return std::reverse_iterator(mData.end()); }
    NDISCARD auto rend() { return std::reverse_iterator(begin()); }
    NDISCARD auto rend() const { return std::reverse_iterator(begin()); }

  private:
    inline void Init(const tchar *pStr, usize nLength)
    {
        if (nLength > 0)
        {
            memcpy(mData.GetData(), pStr, nLength * sizeof(CharType));
            mData[i32(nLength) - 1] = tchar(0);
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

    ArrayType mData;
};

inline namespace Literals
{
inline namespace StringLiterals
{
inline CString operator""_s(const tchar *pStr, usize nLength)
{
    return {pStr, nLength};
}
} // namespace StringLiterals
} // namespace Literals

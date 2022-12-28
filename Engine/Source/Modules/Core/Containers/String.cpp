#include "Encoding/IEncoder.hpp"
#include "WeString.hpp"

namespace WE::Internal
{
template <class LhsType, class RhsType>
inline CString ConcatStrings(LhsType &&lhs, RhsType &&rhs) noexcept
{
    if (lhs.IsEmpty())
    {
        return std::forward<RhsType>(rhs);
    }

    const i32 rhsLength = rhs.GetLength();

    // TODO: Optimize with extra slots (Ticket: #13)
    CString result(std::forward<LhsType>(lhs));
    result.Append(rhs.GetArray().GetData(), rhsLength);
    return result;
}

template <class LhsType>
inline CString ConcatStringRange(LhsType &&lhs, const tchar *pRhs, i32 nRhsLength) noexcept
{
    if (nRhsLength == 0)
    {
        return std::forward<LhsType>(lhs);
    }

    // TODO: Optimize with extra slots (Ticket: #13)
    CString result(std::forward<LhsType>(lhs));
    result.Append(pRhs, nRhsLength);
    return result;
}

template <class RhsType>
inline CString ConcatCStringRange(const tchar *pLhs, i32 nLhsLength, RhsType &&rhs) noexcept
{
    if (nLhsLength == 0)
    {
        return std::forward<RhsType>(rhs);
    }

    const i32 rhsLength = rhs.GetLength();

    CString result;
    result.GetArray().AddSlots(nLhsLength + rhsLength + 1);

    tchar *data = result.GetArray().GetData();
    memcpy(data, pLhs, nLhsLength * sizeof(tchar));
    memcpy(data + nLhsLength, rhs.GetArray().GetData(), rhsLength * sizeof(tchar));

    return result;
}

template <class LhsType>
inline CString ConcatStringsToCString(LhsType &&lhs, const tchar *pRhs) noexcept
{
    if (pRhs == nullptr)
    {
        return std::forward<LhsType>(lhs);
    }
    return ConcatStringRange(std::forward<LhsType>(lhs), pRhs, i32(std::char_traits<tchar>::length(pRhs)));
}

template <class RhsType>
inline CString ConcatCStringToStrings(const tchar *pLhs, RhsType &&rhs) noexcept
{
    if (pLhs == nullptr)
    {
        return std::forward<RhsType>(rhs);
    }
    return ConcatCStringRange(pLhs, i32(std::char_traits<tchar>::length(pLhs)), std::forward<RhsType>(rhs));
}
} // namespace WE::Internal

CString CString::ConcatSS(CString &&lhs, CString &&rhs) noexcept
{
    return WE::Internal::ConcatStrings(std::move(lhs), std::move(rhs));
}

CString CString::ConcatSS(CString &&lhs, const CString &rhs) noexcept
{
    return WE::Internal::ConcatStrings(std::move(lhs), rhs);
}

CString CString::ConcatSS(const CString &lhs, CString &&rhs) noexcept
{
    return WE::Internal::ConcatStrings(lhs, std::move(rhs));
}

CString CString::ConcatSS(const CString &lhs, const CString &rhs)
{
    return WE::Internal::ConcatStrings(lhs, rhs);
}

CString CString::ConcatSC(CString &&lhs, const tchar *pRhs) noexcept
{
    return WE::Internal::ConcatStringsToCString(std::move(lhs), pRhs);
}

CString CString::ConcatSC(const CString &lhs, const tchar *pRhs)
{
    return WE::Internal::ConcatStringsToCString(lhs, pRhs);
}

CString CString::ConcatCS(const tchar *pLhs, CString &&rhs) noexcept
{
    return WE::Internal::ConcatCStringToStrings(pLhs, std::move(rhs));
}

CString CString::ConcatCS(const tchar *pLhs, const CString &rhs)
{
    return WE::Internal::ConcatCStringToStrings(pLhs, rhs);
}

CArray<CString> CString::Split(tchar chr) const
{
    we_assert(chr != 0);

    CArray<CString> result;

    const tchar *pStart = mData.GetData();
    const tchar *pEnd = pStart;
    for (; *pEnd; ++pEnd)
    {
        if (*pEnd == chr and pEnd != pStart)
        {
            result.Add(CString(pStart, pEnd - pStart));
            pStart = pEnd + 1;
        }
    }

    if (pStart != pEnd)
    {
        result.Add(CString(pStart, pEnd - pStart));
    }

    return result;
}

CArray<CString> CString::Split(const tchar *pStr) const
{
    we_assert(pStr != nullptr);

    const i32 length = i32(std::char_traits<tchar>::length(pStr));

    const tchar *pStart = mData.GetData();
    const tchar *pEnd = pStart;

    CArray<CString> result;
    for (; *pEnd; ++pEnd)
    {
        if (CStringUtils::Compare(pEnd, length, pStr) == 0)
        {
            result.Add(CString(pStart, pEnd - pStart));
            pStart = pEnd + 1;
        }
    }

    if (pStart != pEnd)
    {
        result.Add(CString(pStart, pEnd - pStart));
    }

    return result;
}

CString::CString(const u8 *pBytes, usize nSize, const IEncoder *pEncoder)
{
    we_assert(pBytes != nullptr and pEncoder != nullptr);

    const usize length = pEncoder->GetLength(pBytes, nSize);
    if (length > 0)
    {
        mData.AddSlots(i32(length) + 1);
        pEncoder->Decode(mData.GetData(), pBytes, length);
    }
}

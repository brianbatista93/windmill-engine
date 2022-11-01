#include "WeString.hpp"

namespace WE::Internal
{
template <class TLhs, class TRhs>
inline CString ConcatStrings(TLhs &&lhs, TRhs &&rhs) noexcept
{
    if (lhs.IsEmpty())
    {
        return std::forward<TRhs>(rhs);
    }

    const i32 rhsLength = rhs.GetLength();

    // TODO: Optimize with extra slots (Ticket: #13)
    CString result(std::forward<TLhs>(lhs));
    result.Append(rhs.GetArray().GetData(), rhsLength);
    return result;
}

template <class TLhs>
inline CString ConcatStringRange(TLhs &&lhs, const tchar *pRhs, i32 nRhsLength) noexcept
{
    if (nRhsLength == 0)
    {
        return std::forward<TLhs>(lhs);
    }

    // TODO: Optimize with extra slots (Ticket: #13)
    CString result(std::forward<TLhs>(lhs));
    result.Append(pRhs, nRhsLength);
    return result;
}

template <class TRhs>
inline CString ConcatCStringRange(const tchar *pLhs, i32 nLhsLength, TRhs &&rhs) noexcept
{
    if (nLhsLength == 0)
    {
        return std::forward<TRhs>(rhs);
    }

    const i32 rhsLength = rhs.GetLength();

    CString result;
    result.GetArray().AddSlots(nLhsLength + rhsLength + 1);

    tchar *data = result.GetArray().GetData();
    memcpy(data, pLhs, nLhsLength * sizeof(tchar));
    memcpy(data + nLhsLength, rhs.GetArray().GetData(), rhsLength * sizeof(tchar));

    return result;
}

template <class TLhs>
inline CString ConcatStringsToCString(TLhs &&lhs, const tchar *pRhs) noexcept
{
    if (pRhs == nullptr)
    {
        return std::forward<TLhs>(lhs);
    }
    return ConcatStringRange(std::forward<TLhs>(lhs), pRhs, i32(std::char_traits<tchar>::length(pRhs)));
}

template <class TRhs>
inline CString ConcatCStringToStrings(const tchar *pLhs, TRhs &&rhs) noexcept
{
    if (pLhs == nullptr)
    {
        return std::forward<TRhs>(rhs);
    }
    return ConcatCStringRange(pLhs, i32(std::char_traits<tchar>::length(pLhs)), std::forward<TRhs>(rhs));
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
#include "StringBuilder.hpp"
#include "WeString.hpp"

CStringBuilder::CStringBuilder()
{
    m_Data.Reserve(50);
}

CStringBuilder::CStringBuilder(i32 nInitialCapacity)
{
    m_Data.Reserve(nInitialCapacity);
}

CStringBuilder &CStringBuilder::Append(tchar chr)
{
    m_Data.Add(chr);
    return *this;
}

CStringBuilder &CStringBuilder::Append(tchar chr, i32 nCount)
{
    we_assert(nCount != 0);

    for (i32 i = 0; i < nCount; ++i)
    {
        Append(chr);
    }

    return *this;
}

CStringBuilder &CStringBuilder::Append(const tchar *pStr)
{
    const i32 nLength = i32(std::char_traits<tchar>::length(pStr));
    return Append(pStr, nLength);
}

CStringBuilder &CStringBuilder::Append(const tchar *pStr, i32 nLength)
{
    const i32 index = m_Data.AddSlots(nLength);
    memcpy(m_Data.GetData() + index, pStr, nLength * sizeof(tchar));

    return *this;
}

CStringBuilder &CStringBuilder::Append(const CString &str)
{
    const i32 nLength = str.GetLength();
    const i32 index = m_Data.AddSlots(nLength);
    memcpy(m_Data.GetData() + index, *str, nLength * sizeof(tchar));

    return *this;
}

CString CStringBuilder::ToString()
{
    return CString(m_Data.begin(), m_Data.GetSize());
}

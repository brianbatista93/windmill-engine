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

CStringBuilder &CStringBuilder::Append(const tchar *pStr)
{
    const i32 length = i32(std::char_traits<tchar>::length(pStr));
    const i32 index = m_Data.AddSlots(length);
    memcpy(m_Data.GetData() + index, pStr, length * sizeof(tchar));

    return *this;
}

CStringBuilder &CStringBuilder::Append(const CString &str)
{
    const i32 length = str.GetLength();
    const i32 index = m_Data.AddSlots(length);
    memcpy(m_Data.GetData() + index, *str, length * sizeof(tchar));

    return *this;
}

CString CStringBuilder::ToString()
{
    return CString(m_Data.begin(), m_Data.GetSize());
}

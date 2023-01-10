#include "StringBuilder.hpp"
#include "WeString.hpp"

CStringBuilder::CStringBuilder()
{
    mData.Reserve(64);
}

CStringBuilder::CStringBuilder(i32 nInitialCapacity)
{
    mData.Reserve(nInitialCapacity);
}

CStringBuilder &CStringBuilder::Append(tchar chr)
{
    mData.Add(chr);
    return *this;
}

CStringBuilder &CStringBuilder::Append(tchar chr, i32 nCount)
{
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
    const i32 index = mData.AddSlots(nLength);
    memcpy(mData.GetData() + index, pStr, nLength * sizeof(tchar));

    return *this;
}

CStringBuilder &CStringBuilder::Append(const CString &str)
{
    const i32 nLength = str.GetLength();
    const i32 index = mData.AddSlots(nLength);
    memcpy(mData.GetData() + index, *str, nLength * sizeof(tchar));

    return *this;
}

CStringBuilder &CStringBuilder::AppendLine(const CString &str)
{
    Append(str);
    Append(WT('\n'));

    return *this;
}

CString CStringBuilder::ToString()
{
    return {mData.begin(), (usize)mData.GetSize()};
}

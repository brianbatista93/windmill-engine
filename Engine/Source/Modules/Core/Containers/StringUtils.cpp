#include <string>

#include "Hashing/CRC32.hpp"
#include "StringUtils.hpp"

bool CStringUtils::StartsWith(const tchar *__restrict pStr, const tchar *__restrict pStart)
{
    for (; *pStr && *pStart; pStr++, pStart++)
    {
        if (*pStr != *pStart)
        {
            return false;
        }
    }

    return (*pStart == tchar(0));
}

bool CStringUtils::EndsWith(const tchar *__restrict pStr, const tchar *__restrict pEnd, i32 nLength)
{
    const i32 endLength = i32(std::char_traits<tchar>::length(pEnd));
    if (endLength < nLength)
    {
        const tchar *iter = &pStr[nLength - endLength];
        while (*iter)
        {
            if (*iter++ != *pEnd++)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool CStringUtils::EndsWith(const tchar *__restrict pStr, const tchar *__restrict pEnd)
{
    const i32 length = i32(std::char_traits<tchar>::length(pStr));
    return EndsWith(pStr, pEnd, length);
}

i32 CStringUtils::Compare(const tchar *__restrict pLhsStr, const tchar *__restrict pRhsStr)
{
    for (i32 i = 0;; ++i)
    {
        const tchar chr1 = pLhsStr[i];
        const tchar chr2 = pRhsStr[i];

        if (chr1 < chr2)
        {
            return -1;
        }
        if (chr1 > chr2)
        {
            return 1;
        }
        if (chr1 == 0)
        {
            break;
        }
    }

    return 0;
}

i32 CStringUtils::Compare(const tchar *__restrict pLhsStr, i32 nLength, const tchar *__restrict pRhsStr)
{
    for (i32 i = 0; i < nLength; ++i)
    {
        const tchar chr1 = pLhsStr[i];
        const tchar chr2 = pRhsStr[i];

        if (chr1 < chr2)
        {
            return -1;
        }
        if (chr1 > chr2)
        {
            return 1;
        }
        if (chr1 == 0)
        {
            break;
        }
    }

    return 0;
}

i32 CStringUtils::Find(const tchar *__restrict pStr, tchar findChar, i32 nOffset)
{
    const i32 length = i32(std::char_traits<tchar>::length(pStr));

    return Find(pStr, length, findChar, nOffset);
}

i32 CStringUtils::Find(const tchar *__restrict pStr, i32 nStrLength, tchar findChar, i32 nOffset)
{
    pStr += nOffset;
    i32 result = 0;
    for (; pStr && *pStr && result < nStrLength; ++pStr, ++result)
    {
        if (*pStr == findChar)
        {
            return result;
        }
    }

    return -1;
}

i32 CStringUtils::Find(const tchar *__restrict pStr, const tchar *__restrict pFind, i32 nOffset)
{
    const i32 length = i32(std::char_traits<tchar>::length(pStr));
    const i32 findLength = i32(std::char_traits<tchar>::length(pFind));

    return Find(pStr, length, pFind, findLength, nOffset);
}

i32 CStringUtils::Find(const tchar *__restrict pStr, i32 nStrLength, const tchar *__restrict pFind, i32 nFindLength, i32 nOffset)
{
    if (nFindLength > nStrLength)
    {
        return -1;
    }

    for (i32 i = nOffset; i < nStrLength - nFindLength; ++i)
    {
        i32 index;
        for (index = 0; index < nFindLength; ++index)
        {
            if (pStr[i + index] != pFind[index])
            {
                break;
            }
        }

        if (index == nFindLength)
        {
            return i;
        }
    }

    return -1;
}

bool CStringUtils::IsDigit(tchar chr)
{
    return chr >= WT('0') && chr <= WT('9');
}

u64 CStringUtils::GetHash(const tchar *pStr, i32 nLength)
{
    return CRC32::Calculate(pStr, nLength * sizeof(tchar));
}

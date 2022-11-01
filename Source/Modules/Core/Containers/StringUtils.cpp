#include "StringUtils.hpp"
#include <string>

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
    i32 endLength = i32(std::char_traits<tchar>::length(pEnd));
    if (endLength < nLength)
    {
        const tchar *it = &pStr[nLength - endLength];
        while (*it)
        {
            if (*it++ != *pEnd++)
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
        const tchar c1 = pLhsStr[i];
        const tchar c2 = pRhsStr[i];

        if (c1 < c2)
        {
            return -1;
        }
        if (c1 > c2)
        {
            return 1;
        }
        if (c1 == 0)
        {
            break;
        }
    }

    return 0;
}

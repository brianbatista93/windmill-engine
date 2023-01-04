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

#include "Types.hpp"

template <std::unsigned_integral Type>
inline bool TryParse(const tchar *pStr, Type *pOutResult)
{
    we_assert(pOutResult && "pOutResult is null");

    if (!pStr || !*pStr)
    {
        return false;
    }

    Type value = 0;
    while (*pStr != '\0')
    {
        if (*pStr < '0' || *pStr > '9')
        {
            return false;
        }

        value = value * 10 + (Type)(*pStr - WT('0'));
        ++pStr;
    }

    *pOutResult = value;
    return true;
}

template <std::signed_integral Type>
inline bool TryParse(const tchar *pStr, Type *pOutResult)
{
    we_assert(pOutResult && "pOutResult is null");

    if (!pStr || !*pStr)
    {
        return false;
    }

    Type sign = 1;
    if (*pStr == '-')
    {
        sign = -1;
        ++pStr;
    }

    Type value = 0;
    while (*pStr != '\0')
    {
        if (*pStr < '0' || *pStr > '9')
        {
            return false;
        }

        value = value * 10 + (Type)(*pStr - WT('0'));
        ++pStr;
    }

    *pOutResult = value * sign;
    return true;
}

template <std::floating_point Type>
inline bool TryParse(const tchar *pStr, Type *pOutResult)
{
    we_assert(pOutResult && "pOutResult is null");

    if (!pStr || !*pStr)
    {
        return false;
    }

    Type value = 0;
    Type sign = 1;
    if (*pStr == '-')
    {
        sign = -1;
        ++pStr;
    }

    while (*pStr != '\0' && *pStr != '.')
    {
        if (*pStr < '0' || *pStr > '9')
        {
            return false;
        }

        value = value * 10 + (Type)(*pStr - WT('0'));
        ++pStr;
    }

    if (*pStr == '.')
    {
        ++pStr;
        Type fraction = 0;
        Type fractionDivisor = 1;
        while (*pStr != '\0')
        {
            if (*pStr < '0' || *pStr > '9')
            {
                return false;
            }

            fraction = fraction * 10 + (Type)(*pStr - WT('0'));
            fractionDivisor *= 10;
            ++pStr;
        }

        value += fraction / fractionDivisor;
    }

    *pOutResult = value * sign;
    return true;
}

inline bool TryParse(const tchar *pStr, CString *pOutResult)
{
    we_assert(pOutResult && "pOutResult is null");

    if (!pStr || !*pStr)
    {
        return false;
    }

    const usize length = std::char_traits<tchar>::length(pStr);

    if (pStr[0] == WT('\"'))
    {
        if (pStr[length - 1] != WT('\"'))
        {
            return false;
        }

        *pOutResult = {pStr + 1, length - 2};
        return true;
    }

    *pOutResult = pStr;
    return true;
}

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

class CStringUtils
{
  public:
    static bool StartsWith(const tchar *__restrict pStr, const tchar *__restrict pStart);
    static bool EndsWith(const tchar *__restrict pStr, const tchar *__restrict pEnd, i32 nLength);
    static bool EndsWith(const tchar *__restrict pStr, const tchar *__restrict pEnd);

    static i32 Compare(const tchar *__restrict pLhsStr, const tchar *__restrict pRhsStr);
    static i32 Compare(const tchar *__restrict pLhsStr, i32 nLength, const tchar *__restrict pRhsStr);

    inline static bool Equal(const tchar *__restrict pLhsStr, const tchar *__restrict pRhsStr) { return Compare(pLhsStr, pRhsStr) == 0; }
    inline static bool Equal(const tchar *__restrict pLhsStr, const tchar *__restrict pRhsStr, i32 nLength)
    {
        return Compare(pLhsStr, nLength, pRhsStr) == 0;
    }

    static i32 Find(const tchar *__restrict pStr, const tchar *__restrict pFind, i32 nOffset = 0);
    static i32 Find(const tchar *__restrict pStr, i32 nStrLength, const tchar *__restrict pFind, i32 nFindLength, i32 nOffset = 0);

    static bool IsDigit(tchar chr);
};

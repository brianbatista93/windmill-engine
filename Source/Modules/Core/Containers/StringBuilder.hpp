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

#include "Array.hpp"
#include "StringFormatter.hpp"

class CString;

class CStringBuilder
{
  public:
    CStringBuilder();

    CStringBuilder(i32 nInitialCapacity);

    CStringBuilder &Append(tchar chr);

    CStringBuilder &Append(tchar chr, i32 nCount);

    CStringBuilder &Append(const tchar *pStr);

    CStringBuilder &Append(const tchar *pStr, i32 nLength);

    CStringBuilder &Append(const CString &str);

    CStringBuilder &AppendLine(const CString &str);

    template <class... TArgs>
    inline CStringBuilder &AppendFormat(const tchar *pFormat, TArgs &&...vArgs)
    {
        constexpr usize nArgc = sizeof...(vArgs);

        if constexpr (nArgc > 0)
        {
            const CFormatterArgument pArgv[nArgc] = {vArgs...};
            FormatInternal(pFormat, nArgc, pArgv);
        }
        else
        {
            FormatInternal(pFormat, 0, nullptr);
        }

        return *this;
    }

    CString ToString();

  private:
    void FormatInternal(const tchar *pFormat, usize nArgc, const CFormatterArgument *pArgs);

    TArray<tchar, TAllocator<i32>> m_Data;
};
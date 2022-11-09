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

#include <functional>
#include "Types.hpp"

class CString;
class IFormattable;

class CFormatterArgument
{
  public:
    using FormatterFunction = CString (*)(const tchar *pFormat);

    template <class T>
    requires(std::derived_from<T, IFormattable>) CFormatterArgument(const T *a) : m_pFormattable(a) {}

    CFormatterArgument(const tchar *pValue);

    bool TryFormat(tchar **pDest, const tchar *pFormat);

  private:
    const IFormattable *m_pFormattable;

    union {
        i64 m_nDecimal;
        u64 m_nUnsigned;
        f64 m_nDouble;
        const tchar *m_pString;
    };
};
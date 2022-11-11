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

#include "Concepts.hpp"
#include "Text/NumericFormatter.hpp"
#include "Types.hpp"

class CFormatterArgument
{
  public:
    using FormatterFunction = std::function<bool(tchar **, const tchar *)>;

    enum EType
    {
        kInvalid = 0,
        kString = 1,
        kNumeric = 2,
        kFormattable = 3
    };

    template <WE::Concept::IsFormattable T>
    CFormatterArgument(const T &rValue) : m_eType(kFormattable)
    {
        m_pFormatter = [=](tchar **pDest, const tchar *pFormat) {
            return ::TryFormat(rValue, pDest, pFormat);
        };
    }

    template <WE::Concept::IsNumeric T>
    CFormatterArgument(const T &rValue) : m_eType(kNumeric)
    {
        m_pFormatter = [=](tchar **pDest, const tchar *pFormat) {
            return CNumeric::TryFormat<T>(rValue, pDest, pFormat);
        };
    }

    CFormatterArgument(const tchar *pValue);
    CFormatterArgument(class CString value);

    bool TryFormat(tchar **pDest, const tchar *pFormat);

    inline EType GetType() const { return m_eType; }

    inline const tchar *GetString() const { return m_pString; }

  private:
    FormatterFunction m_pFormatter{nullptr};
    EType m_eType{kInvalid};
    const tchar *m_pString;
};
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

#include "Concepts.hpp"
#include "Containers/StringUtils.hpp"

#include <math.h>

class CNumeric
{
  public:
    inline static i32 kDefaultPrecision = 6;
    inline static i32 kDefaultExponentialPrecision = 3;

    inline static tchar kDecimalPoint = WT('.');
    inline static tchar kThousandsSeparator = WT(',');

    enum EType
    {
        kInvalid = 0,
        kDecimal = 1,
        kScientific = 2,
        kFixedPoint = 3,
        kNumber = 4,
        kPercent = 5,
        kHexadecimal = 6
    };

    template <WE::Concept::IsNumeric T>
    inline static bool TryFormat(T value, tchar **pDest, const tchar *pFormat)
    {
        EType eType = kInvalid;
        i32 nPrecision = 0;
        bool bIsUpperCase = true;
        if (!ParseFormat<T>(pFormat, &eType, &nPrecision, &bIsUpperCase))
        {
            return false;
        }

        switch (eType)
        {
        case CNumeric::kDecimal:
            return FormatDecimal(value, pDest, nPrecision);
        case CNumeric::kScientific:
            return FormatScientific(value, pDest, nPrecision, bIsUpperCase);
        case CNumeric::kFixedPoint:
            return FormatFixedPoint(value, pDest, nPrecision);
        case CNumeric::kNumber:
            return FormatNumber(value, pDest, nPrecision);
        case CNumeric::kPercent:
            return FormatPercentage(value, pDest, nPrecision);
        case CNumeric::kHexadecimal:
        default:
            return false;
        }
    }

  private:
    template <class T>
    inline static bool ParseFormat(const tchar *pFormat, EType *pType, i32 *pPrecision, bool *pIsUpperCase)
    {
        i32 nDefaultPrecision = kDefaultPrecision;

        switch (*pFormat)
        {
        case WT('d'):
        case WT('D'):
            *pType = kDecimal;
            break;
        case WT('e'):
        case WT('E'):
            *pType = kScientific;
            break;
        case WT('f'):
        case WT('F'):
            *pType = kFixedPoint;
            nDefaultPrecision = 2;
            break;
        case WT('n'):
        case WT('N'):
            *pType = kNumber;
            nDefaultPrecision = 2;
            break;
        case WT('p'):
        case WT('P'):
            *pType = kPercent;
            nDefaultPrecision = 2;
            break;
        case WT('x'):
        case WT('X'):
            *pType = kHexadecimal;
            break;
        default: {
            if constexpr (std::is_integral_v<T>)
            {
                *pType = kDecimal;
                *pPrecision = 0;
                return true;
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                *pType = kFixedPoint;
            }
            else
            {
                *pType = kInvalid;
            }
        }
        break;
        }

        if (*pType != kInvalid)
        {
            *pIsUpperCase = (*pFormat >= WT('A')) and (*pFormat <= WT('Z'));

            ++pFormat;

            if (*pFormat == WT('\0'))
            {
                *pPrecision = nDefaultPrecision;
            }
            else
            {
                do
                {
                    *pPrecision = *pPrecision * 10 + *pFormat - WT('0');
                    ++pFormat;
                } while (CStringUtils::IsDigit(*pFormat));
            }

            return true;
        }

        return false;
    }

    template <class T>
    inline static tchar *FormatUnsigned(T value, tchar *pDest, i32 *nWrittenCount)
    {
        tchar *pStart = pDest;

        do
        {
            *--pDest = tchar(value % 10) + WT('0');
            value /= 10;
        } while (value != 0);

        if (nWrittenCount != nullptr)
        {
            *nWrittenCount = i32(pStart - pDest);
        }

        return pDest;
    }

    template <class T>
    inline static tchar *FormatSigned(T value, tchar *pDest, i32 *nWrittenCount)
    {
        using TUnsigned = std::make_unsigned_t<T>;
        const TUnsigned nUnsignedValue = (TUnsigned)value;

        if (value < 0)
        {
            pDest = FormatUnsigned(0U - nUnsignedValue, pDest, nWrittenCount);
            *--pDest = WT('-');
        }
        else
        {
            pDest = FormatUnsigned(nUnsignedValue, pDest, nWrittenCount);
        }

        return pDest;
    }

    template <class T>
    inline static bool FormatDecimal(T value, tchar **pDest, i32 nPrecision)
    {
        if constexpr (std::is_integral_v<T>)
        {
            i32 nWrittenCount = 0;

            if (value < 0)
            {
                *pDest = FormatUnsigned(0U - value, *pDest, &nWrittenCount);
            }
            else
            {
                *pDest = FormatUnsigned(value, *pDest, &nWrittenCount);
            }

            i32 nPad = nPrecision * nWrittenCount;
            while (nPad-- > 0)
            {
                *--*pDest = WT('0');
            }

            if (value < 0)
            {
                *--*pDest = WT('-');
            }

            return true;
        }
        else
        {
            UNREF_PARAM(value);
            UNREF_PARAM(pDest);
            UNREF_PARAM(nPrecision);

            return false;
        }
    }

    template <class T>
    inline static bool FormatScientific(T value, tchar **pDest, i32 nPrecision, bool bIsUpperCase)
    {
        i32 nExpCount = 0;

        f64 doubleValue = (double)value;

        while (doubleValue > 10.0)
        {
            doubleValue *= 0.1;
            ++nExpCount;
        }

        while (doubleValue < 1.0)
        {
            doubleValue *= 10.0;
            --nExpCount;
        }

        const tchar expChar = bIsUpperCase ? WT('E') : WT('e');

        if (FormatDecimal(nExpCount, pDest, kDefaultExponentialPrecision))
        {
            if (nExpCount > 0)
            {
                *--*pDest = WT('+');
            }

            *--*pDest = expChar;

            return FormatFixedPoint(doubleValue, pDest, nPrecision);
        }

        return false;
    }

    template <class T>
    inline static bool FormatFixedPoint(T value, tchar **pDest, i32 nPrecision)
    {
        f64 integerPart = 0;
        f64 decimalPart = modf((f64)value, &integerPart);
        i64 integer = i64(integerPart);
        decimalPart = value < 0 ? (0.0 - decimalPart) : decimalPart;

        if (nPrecision > 0)
        {
            if (decimalPart != 0)
            {
                decimalPart *= pow(10.0, nPrecision);
                u64 decimal = u64(round(decimalPart));

                *pDest = FormatUnsigned(decimal, *pDest, nullptr);
            }
            else
            {
                while (nPrecision-- > 0)
                {
                    *--*pDest = WT('0');
                }
            }

            *--*pDest = WT('.');
        }

        *pDest = FormatSigned(integer, *pDest, nullptr);

        return true;
    }

    template <class T>
    inline bool FormatNumber(T value, tchar **pDest, i32 nPrecision)
    {
        if constexpr (WE::Concept::IsNumeric<T>)
        {
            f64 doubleValue = f64(value);
            f64 integerPart = 0;
            f64 decimalPart = modf(doubleValue, &integerPart);
            i64 integer = i64(integerPart);

            if (nPrecision > 0)
            {
                if (decimalPart != 0)
                {
                    decimalPart *= pow(10.0, nPrecision);
                    u64 decimal = u64(round(decimalPart));

                    *pDest = FormatUnsigned(decimal, *pDest, nullptr);
                }
                else
                {
                    while (nPrecision-- > 0)
                    {
                        *--pDest = WT('0');
                    }
                }

                *--pDest = kDecimalPoint;
            }

            i32 nWrittenCount = 0;

            do
            {
                *--pDest = tchar(integer % 10) + WT('0');
                integer /= 10;

                if (nWrittenCount++ % 3 == 2)
                {
                    *--pDest = kThousandsSeparator;
                }
            } while (integer != 0);

            return true;
        }
        else
        {
            return false;
        }
    }

    template <class T>
    inline static bool FormatPercentage(T value, tchar **pDest, i32 nPrecision)
    {
        *--*pDest = WT('\045');
        return FormatNumber(value * 100.0, pDest, nPrecision);
    }
};
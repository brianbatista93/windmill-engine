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

#include <concepts>

/**
 * @brief Collection of math functions
 */
class CMath
{
  public:
    /**
     * @brief Checks if a number is a power of two
     */
    static constexpr bool IsPowerOfTwo(auto value) { return (value & (value - 1)) == 0; }

    template <class T>
    static constexpr std::make_signed_t<T> Abs(T value)
    {
        const std::make_signed_t<T> sign = value;
        return sign < 0 ? -sign : sign;
    }

    template <class T>
    static constexpr T Min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <class T>
    static constexpr T Max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    static constexpr T GetPrime(T value)
    {
        if constexpr (std::is_signed_v<T>)
        {
            if (value < 0)
            {
                return -GetPrime(-value);
            }
        }

        if (value < 3)
        {
            return value;
        }

        if (value % 2 == 0)
        {
            value++;
        }

        while (true)
        {
            bool is_prime = true;

            for (T i = 3; i * i <= value; i += 2)
            {
                if (value % i == 0)
                {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime)
            {
                return value;
            }

            value += 2;
        }
    }
};

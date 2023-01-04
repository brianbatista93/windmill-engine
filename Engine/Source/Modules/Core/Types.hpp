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

#include <cstddef>
#include <cstdint>
#include <utility>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using usize = size_t;

using ansi = char;
using wide = wchar_t;
#ifdef __cpp_char8_t
using utf8 = char8_t;
#else
using utf8 = ansi;
#endif // __cpp_char8_t

using utf16 = char16_t;
using utf32 = char32_t;

#ifdef UNICODE
using tchar = wide;
    #define WT(x) L##x
    #define WTL(x) L##x##_s
#else
using tchar = ansi;
    #define WT(x) x
    #define WTL(x) x##_s
#endif // WE_OS_WINDOWS

template <class Type>
class CType
{
};

#define DECLARE_TYPE(type, name, shortName)                                                                                                          \
    template <>                                                                                                                                      \
    class CType<type>                                                                                                                                \
    {                                                                                                                                                \
      public:                                                                                                                                        \
        inline static const char *GetName()                                                                                                          \
        {                                                                                                                                            \
            return name;                                                                                                                             \
        }                                                                                                                                            \
        inline static const char *GetShortName()                                                                                                     \
        {                                                                                                                                            \
            return shortName;                                                                                                                        \
        }                                                                                                                                            \
        inline static usize GetSize()                                                                                                                \
        {                                                                                                                                            \
            return sizeof(type);                                                                                                                     \
        }                                                                                                                                            \
        inline static usize GetAlign()                                                                                                               \
        {                                                                                                                                            \
            return alignof(type);                                                                                                                    \
        }                                                                                                                                            \
        inline friend u64 GetHash(type value)                                                                                                        \
        {                                                                                                                                            \
            return static_cast<u64>(value);                                                                                                          \
        }                                                                                                                                            \
    }

DECLARE_TYPE(u8, "uint8", "u8");
DECLARE_TYPE(u16, "uint16", "u16");
DECLARE_TYPE(u32, "uint32", "u32");
DECLARE_TYPE(u64, "uint64", "u64");

DECLARE_TYPE(i8, "int8", "i8");
DECLARE_TYPE(i16, "int16", "i16");
DECLARE_TYPE(i32, "int32", "i32");
DECLARE_TYPE(i64, "int64", "i64");

DECLARE_TYPE(f32, "float", "f32");
DECLARE_TYPE(f64, "double", "f64");

DECLARE_TYPE(wide, "wide", "wide");
DECLARE_TYPE(utf16, "utf16", "utf16");
DECLARE_TYPE(utf32, "utf32", "utf32");

#ifdef __cpp_char8_t
DECLARE_TYPE(utf8, "utf8", "utf8");
#endif // __cpp_char8_t

#include <cassert>

#ifndef we_assert
    #define we_assert(x) assert((x))
#endif // !we_assert

template <class Type>
constexpr void SetIfNotNull(Type *output, Type value)
{
    if (output != nullptr)
    {
        *output = value;
    }
}

#if defined(_MSC_VER) && !defined(__clang__)
    #if !defined(_DEBUG) && !defined(NDEBUG)
        #define NDEBUG
    #endif
    #define UNREF_PARAM(x) (x)
#elif defined(__GNUC__) || defined(__clang__)
    #define UNREF_PARAM(x) ((void)(x))
#endif

#define NDISCARD [[nodiscard]]

template <class KeyType, class ValueType>
inline u64 GetHash(std::pair<KeyType, ValueType> pair)
{
    return GetHash(pair.first);
}

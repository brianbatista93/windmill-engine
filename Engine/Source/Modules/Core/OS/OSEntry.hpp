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

#include "OS/StringConvertion.hpp"
#include "Types.hpp"

#ifdef WE_OS_WINDOWS
    #define MAIN_ENTRY_BEGIN                                                                                                                         \
        i32 wmain(i32 nArgC, wide **ppArgV)                                                                                                          \
        {

    #define MAIN_ENTRY_END }
#else
    #define MAIN_ENTRY_BEGIN                                                                                                                         \
        i32 main(i32 nArgC, ansi **ppAnsiArgV)                                                                                                       \
        {                                                                                                                                            \
            i32 errorCode = 0;                                                                                                                       \
            tchar *ppArgV[nArgC];                                                                                                                    \
            for (i32 i = 0; i < nArgC; ++i)                                                                                                          \
            {                                                                                                                                        \
                auto cast = CStringCast<tchar, ansi>(ppAnsiArgV[i]);                                                                                 \
                const usize length = cast.GetLength() + 1;                                                                                           \
                ppArgV[i] = (tchar *)we_malloc(length * sizeof(tchar));                                                                              \
                memcpy(ppArgV[i], *cast, length * sizeof(tchar));                                                                                    \
            }

    #define MAIN_ENTRY_END                                                                                                                           \
        for (i32 i = 0; i < nArgC; ++i)                                                                                                              \
        {                                                                                                                                            \
            we_free(ppArgV[i]);                                                                                                                      \
        }                                                                                                                                            \
        return errorCode;                                                                                                                            \
        }
#endif // WE_OS_WINDOWS

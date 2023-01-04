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
#include <string>

namespace OS
{
extern usize TCharToUTF8(utf8 *pDst, const tchar *pSrc, usize nSrcLength);

extern usize UTF8ToTChar(tchar *pDst, const utf8 *pSrc, usize nSrcLength);
} // namespace OS

template <class ToType, class FromType>
class CStringCast
{
};

template <>
class CStringCast<ansi, wide>
{
  public:
    inline CStringCast(const wide *pSrc) : CStringCast(pSrc, std::char_traits<wide>::length(pSrc)) {}
    inline CStringCast(const wide *pSrc, usize nLength) : mLength(nLength)
    {
        we_assert(nLength < 1024);
        OS::TCharToUTF8((utf8 *)mBuffer, (const tchar *)pSrc, nLength);
    }

    inline const ansi *operator*() const { return mBuffer; }
    NDISCARD inline usize GetLength() const { return mLength; }

  private:
    ansi mBuffer[1024]{0};
    usize mLength;
};

template <>
class CStringCast<wide, ansi>
{
  public:
    inline CStringCast(const ansi *pSrc) : CStringCast(pSrc, std::char_traits<ansi>::length(pSrc)) {}
    inline CStringCast(const ansi *pSrc, usize nLength) : mLength(nLength)
    {
        we_assert(nLength < 1024);
        OS::UTF8ToTChar((tchar *)mBuffer, (const utf8 *)pSrc, nLength);
    }

    inline const wide *operator*() const { return mBuffer; }
    NDISCARD inline usize GetLength() const { return mLength; }

  private:
    wide mBuffer[1024]{0};
    usize mLength;
};

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

#include "Containers/StringView.hpp"

class CLineIterator
{
  public:
    CLineIterator(const CStringView &view) noexcept : mView(view) {}

    bool GetLine(CStringView &line)
    {
        if (mView.IsEmpty())
        {
            return false;
        }

        usize extraChars = 0;
        tchar *oldBegin = mView.begin();
        tchar *begin = mView.begin();
        tchar *end = mView.end();

        for (; begin != end; ++begin)
        {
            if (*begin == '\r')
            {
                ++extraChars;
                continue;
            }

            if (*begin == '\n')
            {
                ++begin;
                ++extraChars;
                break;
            }
        }

        mView = {begin, end};

        line = {oldBegin, begin - extraChars};

        return true;
    }

  private:
    CStringView mView;
};

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

#include "IEncoder.hpp"

class CUTF8Encoder : public IEncoder
{
  public:
    static CUTF8Encoder &Get();

    virtual const tchar *GetName() override { return WT("UTF-8"); }
    virtual const tchar *GetShortName() override { return WT("utf-8"); }
    virtual usize GetLength(const u8 *pBytes, usize nByteCount) const override;
    virtual usize Decode(tchar *pDest, const u8 *pSrc, usize nByteCount) const override;

    usize Encode(utf8 *pDest, const tchar *pSrc, usize nByteCount) const;

  private:
    CUTF8Encoder() = default;
};
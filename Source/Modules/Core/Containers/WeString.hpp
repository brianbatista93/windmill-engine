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

#include "Allocator.hpp"
#include "Array.hpp"

class CString
{
  public:
    using CharType = char;
    using ArrayType = TArray<CharType, TAllocator<i32>>;

    CString() = default;
    CString(CString &&) = default;
    CString(const CString &) = default;
    CString &operator=(CString &&) = default;
    CString &operator=(const CString &) = default;

    inline i32 GetLength() const { return m_Data.GetSize() > 0 ? (m_Data.GetSize() - 1) : 0; }
    inline bool IsEmpty() const { return GetLength() == 0; }

  private:
    ArrayType m_Data;
};
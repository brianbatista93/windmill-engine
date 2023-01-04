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

#include "Containers/ContainerFwd.hpp"
#include "Containers/WeString.hpp"
#include "HAL/Path.hpp"

class IFileNative;

class CFile
{
  public:
    enum class EEncoding
    {
        eAuto = 0,
        eAnsi,
        eUnicode,
        eUTF8,
        eUTF8NoBOM
    };

    static bool WriteBytes(const CArray<u8> &bytes, const CPath &filename);
    static bool WriteString(const CString &str, const CPath &filename, EEncoding encoding = EEncoding::eAuto);
    static bool WriteString(IFileNative *pFile, const CString &str, EEncoding encoding = EEncoding::eAuto);

    static bool ReadBytes(CArray<u8> &bytes, const CPath &filename);
    static bool ReadString(CString &result, const CPath &filename);

    static bool IsValid(const class IFileNative *pFile);
    static bool Exist(const CPath &filename);
};

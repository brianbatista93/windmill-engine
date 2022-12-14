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

#include "HAL/Interfaces/IFileSystem.hpp"

class CLinuxFileSystem final : public IFileSystem
{
  public:
    NDISCARD bool FileExists(const CPath &path) const override;
    NDISCARD bool DirectoryExists(const CPath &path) const override;
    NDISCARD bool CreateDirectory(const CPath &path) const override;
    NDISCARD IFileNative *OpenRead(const CPath &filename, bool bCanWrite) override;
    NDISCARD IFileNative *OpenWrite(const CPath &filename, bool bAppend, bool bCanRead) override;
    NDISCARD CArray<CPath> ListChildren(const CPath &path) const override;
};

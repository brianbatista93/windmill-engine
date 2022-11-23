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

#include "Interfaces/IFileSystem.hpp"

class CFileSystem
{
  public:
    inline static bool Initialize() { return Get()->Initialize(); }
    inline static void Shutdown() { Get()->Initialize(); }
    inline static bool FileExists(const CPath &path) { return Get()->FileExists(path); }
    inline static bool DirectoryExists(const CPath &path) { return Get()->DirectoryExists(path); }
    inline static bool CreateDirectory(const CPath &path) { return Get()->CreateDirectory(path); }
    inline static IFileNative *OpenRead(const CPath &path, bool bCanWrite = false) { return Get()->OpenRead(path, bCanWrite); }
    inline static IFileNative *OpenWrite(const CPath &path, bool bAppend = false, bool bCanRead = false)
    {
        return Get()->OpenWrite(path, bAppend, bCanRead);
    }

  private:
    static class IFileSystem *Get();
};
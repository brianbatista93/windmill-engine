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

#include "HAL/Path.hpp"

enum class EResourceMountType
{
    eAssets = 0, // Bundle files
    eDebug,      // Logs and other debug files
    eHome,       // Home (Documents) directory
#ifdef WE_OS_ANDROID
    eSystem, // /proc/
#endif       // WE_OS_ANDROID
    eCount
};

class IFileNative
{
  public:
    virtual ~IFileNative() = default;
    virtual bool IsValid() const = 0;
    virtual usize GetSize() const = 0;
    virtual void Close() = 0;
    virtual void Flush() = 0;
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual void Seek(usize nPosition, i32 mode) = 0;
    virtual usize Read(u8 *pBytes, usize nSize) = 0;
    virtual usize Write(const u8 *pBytes, usize nSize) = 0;
};

class IFileSystem
{
  public:
    virtual ~IFileSystem() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual bool FileExists(const CPath &path) = 0;
    virtual bool DirectoryExists(const CPath &path) = 0;
    virtual bool CreateDirectory(const CPath &path) = 0;
    virtual IFileNative *OpenRead(const CPath &filename, bool bCanWrite = false) = 0;
    virtual IFileNative *OpenWrite(const CPath &filename, bool bAppend = false, bool bCanRead = false) = 0;

  protected:
    CPath m_MountedDirs[(u32)EResourceMountType::eCount];
};
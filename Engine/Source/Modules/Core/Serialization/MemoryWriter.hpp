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

#include "BinaryArchive.hpp"
#include "Containers/Array.hpp"
#include "Containers/ContainerFwd.hpp"

class CMemoryWriter final : public IBinaryArchive
{
  public:
    CMemoryWriter(i32 nInitialSize) : mBuffer(nInitialSize) {}

    const tchar *GetArchiveName() const final { return WT("MemoryWriter"); }

    bool IsReading() const final { return false; }

    bool IsWriting() const final { return true; }

    bool ForceByteSwapping() const final { return false; }

    void Serialize(void *pData, usize nSize) final;

    NDISCARD inline i32 GetSize() const { return mOffset; }

  private:
    CArray<u8> mBuffer;
    i32 mOffset{0};
};

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

#include "Concepts.hpp"
#include "Types.hpp"

class CString;

class CArchive
{
  public:
    virtual ~CArchive();

    virtual void Serialize(void * /*pData*/, usize /*nSize*/) {}

    inline friend CArchive &operator<<(CArchive &archive, WE::Concept::IsNumeric auto &value)
    {
        if constexpr (sizeof(value) > 1)
        {
            archive.EndianSerialize(&value, sizeof(value));
        }
        else
        {
            archive.Serialize(&value, sizeof(value));
        }

        return archive;
    }

    inline friend CArchive &operator<<(CArchive &archive, bool &value)
    {
        archive.Serialize(&value, 1);
        return archive;
    }

    friend CArchive &operator<<(CArchive &archive, CString &value);

    inline NDISCARD bool IsReading() const { return mIsReading; }

    inline NDISCARD bool IsWriting() const { return mIsWriting; }

    inline NDISCARD bool IsPersistent() const { return mIsPersistent; }

    inline NDISCARD bool IsByteSwapping() const
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            return mForceByteSwapping;
        }
        else
        {
            return IsPersistent();
        }
    }

    inline NDISCARD u32 GetPackageVersion() const { return mPackageVersion; }

    inline NDISCARD u32 GetEngineVersion() const { return mEngineVersion; }

    inline NDISCARD u32 GetNetworkVersion() const { return mNetworkVersion; }

    inline NDISCARD bool HasErrors() const { return mHasErrors; }

    void ClearErrors();

  protected:
    void EndianSerialize(void *pData, usize nSize);

    /** Whether this archive is reading data. */
    bool mIsReading{false};

    /** Whether this archive is writing data. */
    bool mIsWriting{false};

    /** Wheter this archive persists data. */
    bool mIsPersistent{false};

    /** Whether we should forcefully swap bytes. */
    bool mForceByteSwapping{false};

    u32 mPackageVersion;
    u32 mEngineVersion;
    u32 mNetworkVersion;

  private:
    /** Whether this archive has errors. */
    bool mHasErrors;
};

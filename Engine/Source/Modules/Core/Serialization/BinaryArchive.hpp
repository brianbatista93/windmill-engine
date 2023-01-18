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

#include "IArchive.hpp"

class IBinaryArchive : public IArchive
{
  public:
    virtual ~IBinaryArchive() = default;

    virtual bool ForceByteSwapping() const = 0;
    virtual void Serialize(void *pData, usize nSize) = 0;

    NDISCARD bool IsPersistent() const override { return false; }

    NDISCARD bool IsByteSwapping() const
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            return ForceByteSwapping();
        }
        else
        {
            return IsPersistent();
        }
    }

    inline friend IBinaryArchive &operator<<(IBinaryArchive &archive, WE::Concept::IsNumeric auto &value)
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

    inline friend IBinaryArchive &operator<<(IBinaryArchive &archive, bool &value)
    {
        archive.Serialize(&value, 1);
        return archive;
    }

    friend IBinaryArchive &operator<<(IBinaryArchive &archive, CString &value);

  protected:
    void EndianSerialize(void *pData, usize nSize);
};

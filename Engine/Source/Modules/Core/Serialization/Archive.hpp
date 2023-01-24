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
    virtual const tchar *GetArchiveName() const { return WT("Unnamed Archive"); }

    virtual bool IsReading() const { return false; }

    virtual bool IsWriting() const { return false; }

    virtual bool IsPersistent() const { return false; }

    virtual bool ForceByteSwapping() const { return false; }

    virtual void BeginObject(const tchar * /*pName*/ = nullptr) {}
    virtual void EndObject() {}

    virtual void BeginSequence(const tchar * /*pName*/ = nullptr) {}
    virtual void EndSequence() {}

    virtual void Serialize(void * /*pData*/, usize /*nSize*/, const tchar * /*pName*/ = nullptr) {}

    virtual void SerializeBoolean(bool &value, const tchar *pName) { Serialize(&value, 1u, pName); }
    virtual void SerializeString(CString & /*str*/, const tchar * /*pName*/) {}

    bool HasErrors() const { return mHasErros; }

    void SetError() { mHasErros = true; }

    void ClearErrors() { mHasErros = false; }

    inline bool NeedSwapping() const
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

    CArchive &operator()(we::concepts::IsNumeric auto &value, const tchar *pName = nullptr) { return SerializeNumber(value, pName); }

    CArchive &operator()(bool &value, const tchar *pName = nullptr)
    {
        SerializeBoolean(value, pName);
        return *this;
    }

    CArchive &operator()(CString &str, const tchar *pName = nullptr)
    {
        SerializeString(str, pName);
        return *this;
    }

  private:
    void EndianSerialize(void *pValue, usize nSize, const tchar *pName);

    CArchive &SerializeNumber(we::concepts::IsNumeric auto &value, const tchar *pName = nullptr)
    {
        if constexpr (sizeof(value) > 1)
        {
            EndianSerialize(&value, sizeof(value), pName);
        }
        else
        {
            Serialize(&value, sizeof(value), pName);
        }

        return *this;
    }

  private:
    bool mHasErros{false};
};

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
class IArchive;

// clang-format off
template <class Type>
concept IsSerializable = requires(Type& value, IArchive& archive)
{
	{ archive(value) }  -> std::same_as<IArchive&>;
};
// clang-format on

class IArchive
{
  public:
    virtual ~IArchive() = default;

    virtual const tchar *GetArchiveName() const = 0;

    virtual bool IsReading() const = 0;

    virtual bool IsWriting() const = 0;

    virtual bool IsPersistent() const = 0;

    NDISCARD inline u32 GetPackageVersion() const { return mPackageVersion; }

    NDISCARD inline u32 GetEngineVersion() const { return mEngineVersion; }

    NDISCARD inline u32 GetNetworkVersion() const { return mNetworkVersion; }

    NDISCARD inline bool HasErrors() const { return mHasErrors; }

    inline bool SetError() { mHasErrors = true; }

    inline void ClearErrors() { mHasErrors = 0; }

  protected:
    u32 mPackageVersion;
    u32 mEngineVersion;
    u32 mNetworkVersion;

  private:
    /** Whether this archive has errors. */
    bool mHasErrors{false};
};

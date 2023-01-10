#include "File.hpp"
#include "Containers/Array.hpp"
#include "Encoding/AnsiEncoder.hpp"
#include "Encoding/UTF8Encoder.hpp"
#include "HAL/FileSystem.hpp"
#include "Memory/Memory.hpp"

bool CFile::WriteBytes(const CArray<u8> &bytes, const CPath &filename)
{
    TUniquePtr<IFileNative> fileNative;
    fileNative.reset(CFileSystem::OpenWrite(filename));

    if (!fileNative or !fileNative->IsValid())
    {
        return false;
    }

    const auto size = bytes.GetSize();
    return (i32)fileNative->Write(bytes.GetData(), size) == size;
}

constexpr bool IsAnsi(const tchar *pStr)
{
    for (; *pStr; pStr++)
    {
        if (*pStr > 0x7F)
        {
            return false;
        }
    }
    return true;
}

bool CFile::WriteString(const CString &str, const CPath &filename, EEncoding encoding)
{
    TUniquePtr<IFileNative> fileNative;
    fileNative.reset(CFileSystem::OpenWrite(filename));

    return WriteString(fileNative.get(), str, encoding, false);
}

bool CFile::WriteString(IFileNative *pFile, const CString &str, EEncoding encoding, bool bAppend)
{
    if (str.IsEmpty() or !IsValid(pFile) or !pFile->CanWrite())
    {
        return false;
    }

    if (bAppend)
    {
        pFile->Seek(0, SEEK_END);
    }

    const bool forceUnicode = encoding == EEncoding::eUnicode or (encoding == EEncoding::eAuto and !IsAnsi(*str));
    if (encoding == EEncoding::eUTF8 or encoding == EEncoding::eUTF8NoBOM)
    {
        if (encoding == EEncoding::eUTF8)
        {
            utf8 utf8BOM[] = {(utf8)0xEF, (utf8)0xBB, (utf8)0xBF};
            if (pFile->Write((const u8 *)utf8BOM, 3 * sizeof(utf8)) != 3 * sizeof(utf8))
            {
                return false;
            }
        }
        const usize length = CUTF8Encoder::Encode(nullptr, *str, str.GetLength() * sizeof(tchar));
        CArray<utf8> utf8Str((i32)length);
        CUTF8Encoder::Encode(utf8Str.GetData(), *str, str.GetLength() * sizeof(tchar));
        return pFile->Write((const u8 *)utf8Str.GetData(), length) == length;
    }

    if (forceUnicode)
    {
        const auto unicodeBOM = (tchar)0xFEFF;
        if (pFile->Write((const u8 *)&unicodeBOM, sizeof(tchar)) != sizeof(tchar))
        {
            return false;
        }
        const i32 length = str.GetLength() * (i32)sizeof(tchar);
        return (i32)pFile->Write((const u8 *)*str, length) == length;
    }

    const usize length = CAnsiEncoder::Encode(nullptr, *str, str.GetLength() * sizeof(tchar));
    CArray<ansi> ansiStr((i32)length);
    CAnsiEncoder::Encode(ansiStr.GetData(), *str, str.GetLength() * sizeof(tchar));
    return pFile->Write((const u8 *)ansiStr.GetData(), length) == length;
}

bool CFile::ReadBytes(CArray<u8> &bytes, const CPath &filename)
{
    TUniquePtr<IFileNative> fileNative;
    fileNative.reset(CFileSystem::OpenRead(filename));

    if (!CFile::IsValid(fileNative.get()))
    {
        return false;
    }

    const usize size = fileNative->GetSize();
    bytes.Resize((i32)size);
    return fileNative->Read(bytes.GetData(), size) == size;
}

bool CFile::ReadString(CString &result, const CPath &filename)
{
    TUniquePtr<IFileNative> fileNative;
    fileNative.reset(CFileSystem::OpenRead(filename));
    if (!CFile::IsValid(fileNative.get()))
    {
        return false;
    }

    usize size = fileNative->GetSize();
    if (size == 0)
    {
        result = {};
        return true;
    }

    u8 *bufferMem = (u8 *)we_calloc(size, sizeof(u8));
    u8 *buffer = bufferMem;
    if (fileNative->Read(buffer, size) != size)
    {
        return false;
    }

    auto &resultArray = result.GetArray();

    if ((size >= 2 && !(size & 1) && buffer[0] == 0xFF && buffer[1] == 0xFE) or (size >= 2 && !(size & 1) && buffer[0] == 0xFE && buffer[1] == 0xFF))
    {
        resultArray.AddSlots(i32(size >> 1));
    }
    else
    {
        if (size >= 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF)
        {
            buffer += 3;
            size -= 3;
        }

        const usize length = CUTF8Encoder::Decode(nullptr, buffer, size);
        resultArray.AddSlots(i32(length + 1));
        CUTF8Encoder::Decode(resultArray.GetData(), buffer, size);
    }

    we_free(bufferMem);
    buffer = nullptr;
    bufferMem = nullptr;

    return true;
}

bool CFile::IsValid(const IFileNative *pFile)
{
    return pFile and pFile->IsValid();
}

bool CFile::Exist(const CPath &filename)
{
    return CFileSystem::FileExists(filename);
}

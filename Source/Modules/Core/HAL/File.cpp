#include <memory>

#include "Containers/Array.hpp"
#include "Encoding/AnsiEncoder.hpp"
#include "Encoding/UTF8Encoder.hpp"
#include "File.hpp"
#include "HAL/FileSystem.hpp"

template <class T>
class TDeleter
{
  public:
    constexpr void operator()(T *pObject) const noexcept { we_delete(pObject); }
};

bool CFile::WriteBytes(const TArray<u8> &bytes, const CPath &filename)
{
    std::unique_ptr<IFileNative, TDeleter<IFileNative>> fileNative;
    fileNative.reset(CFileSystem::OpenWrite(filename));

    if (!fileNative or !fileNative->IsValid())
    {
        return false;
    }

    const auto size = bytes.GetSize();
    return fileNative->Write(bytes.GetData(), size) == size;
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
    std::unique_ptr<IFileNative, TDeleter<IFileNative>> fileNative;
    if (str.IsEmpty())
    {
        return true;
    }

    fileNative.reset(CFileSystem::OpenWrite(filename));

    if (!fileNative or !fileNative->IsValid())
    {
        return false;
    }

    const bool forceUnicode = encoding == EEncoding::eUnicode or (encoding == EEncoding::eAuto and !IsAnsi(*str));
    if (encoding == EEncoding::eUTF8 or encoding == EEncoding::eUTF8NoBOM)
    {
        if (encoding == EEncoding::eUTF8)
        {
            utf8 utf8BOM[] = {(utf8)0xEF, (utf8)0xBB, (utf8)0xBF};
            if (fileNative->Write((const u8 *)utf8BOM, 3 * sizeof(utf8)) != 3 * sizeof(utf8))
            {
                return false;
            }
        }
        const usize length = CUTF8Encoder::Get().Encode(nullptr, *str, str.GetLength() * sizeof(tchar));
        TArray<utf8> utf8Str(length);
        CUTF8Encoder::Get().Encode(utf8Str.GetData(), *str, str.GetLength() * sizeof(tchar));
        return fileNative->Write((const u8 *)utf8Str.GetData(), length) == length;
    }
    else if (forceUnicode)
    {
        const tchar unicodeBOM = 0xFEFF;
        if (fileNative->Write((const u8 *)&unicodeBOM, sizeof(tchar)) != sizeof(tchar))
        {
            return false;
        }
        const i32 length = str.GetLength() * sizeof(tchar);
        return fileNative->Write((const u8 *)*str, length) == length;
    }
    else
    {
        const usize length = CAnsiEncoder::Get().Encode(nullptr, *str, str.GetLength() * sizeof(tchar));
        TArray<ansi> ansiStr(length);
        CAnsiEncoder::Get().Encode(ansiStr.GetData(), *str, str.GetLength() * sizeof(tchar));
        return fileNative->Write((const u8 *)ansiStr.GetData(), length) == length;
    }

    return true;
}

bool CFile::ReadBytes(TArray<u8> &bytes, const CPath &filename)
{
    std::unique_ptr<IFileNative, TDeleter<IFileNative>> fileNative;
    fileNative.reset(CFileSystem::OpenRead(filename));

    if (!fileNative or !fileNative->IsValid())
    {
        return false;
    }

    const auto size = fileNative->GetSize();
    bytes.Resize(size);
    return fileNative->Read(bytes.GetData(), size) == size;
}

bool CFile::ReadString(CString &result, const CPath &filename)
{
    std::unique_ptr<IFileNative, TDeleter<IFileNative>> fileNative;
    fileNative.reset(CFileSystem::OpenRead(filename));
    if (!fileNative or !fileNative->IsValid())
    {
        return false;
    }

    usize size = fileNative->GetSize();

    u8 *bufferMem = (u8 *)we_calloc(size, sizeof(u8));
    u8 *buffer = bufferMem;
    if (fileNative->Read(buffer, size) != size)
    {
        return false;
    }

    auto &resultArray = result.GetArray();

    if (size >= 2 && !(size & 1) && buffer[0] == 0xFF && buffer[1] == 0xFE)
    {
        resultArray.AddSlots(i32(size >> 1));
    }
    else if (size >= 2 && !(size & 1) && buffer[0] == 0xFE && buffer[1] == 0xFF)
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

        usize length = CUTF8Encoder::Get().Decode(nullptr, buffer, size);
        resultArray.AddSlots(i32(length + 1));
        CUTF8Encoder::Get().Decode(resultArray.GetData(), buffer, size);
    }

    we_free(bufferMem);
    buffer = nullptr;
    bufferMem = nullptr;

    return true;
}

#include "WindowsInclude.hpp"
#include <shlobj.h>

#include "HAL/FileSystem.hpp"
#include "Memory/Memory.hpp"
#include "WindowsFileSystem.hpp"

IFileSystem *CFileSystem::Get()
{
    static CWindowsFileSystem sInstance;
    return &sInstance;
}

bool CWindowsFileSystem::FileExists(const CPath &path) const
{
    const DWORD attributes = ::GetFileAttributesW(*path);
    return (attributes != INVALID_FILE_ATTRIBUTES) and (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool CWindowsFileSystem::DirectoryExists(const CPath &path) const
{
    const DWORD attributes = ::GetFileAttributesW(*path);
    return (attributes != INVALID_FILE_ATTRIBUTES) and (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool CWindowsFileSystem::CreateDirectory(const CPath &path) const
{
    if (DirectoryExists(path))
    {
        return true;
    }

    const CPath parentPath = path.GetParentPath();
    if (!parentPath.IsEmpty() and !DirectoryExists(parentPath))
    {
        CreateDirectory(parentPath);
    }

    return ::CreateDirectoryW(*path, nullptr);
}

class CWindowsFileNative : public IFileNative
{
    friend CMemoryManager;
    friend CWindowsFileSystem;

  public:
    ~CWindowsFileNative() override { Close(); }

    NDISCARD bool IsValid() const override { return mHandle != nullptr and mHandle != INVALID_HANDLE_VALUE; }
    NDISCARD usize GetSize() const override { return mSize; }
    void Close() override
    {
        if (IsValid())
        {
            Flush();
            if (::CloseHandle(mHandle) == TRUE)
            {
                mHandle = nullptr;
            }
        }
        mCanRead = false;
        mCanWrite = false;
        mSize = 0;
        mPosition = 0;
    }
    void Flush() override { ::FlushFileBuffers(mHandle); }
    NDISCARD bool CanRead() const override { return mCanRead; }
    NDISCARD bool CanWrite() const override { return mCanWrite; }
    void Seek(usize nPosition, i32 mode) override
    {
        we_assert(IsValid());

        switch (mode)
        {
        case SEEK_SET:
            we_assert(nPosition >= 0);
            mPosition = nPosition;
            break;
        case SEEK_CUR:
            mPosition += nPosition;
            break;
        case SEEK_END:
            we_assert(nPosition <= 0);
            mPosition = mSize + nPosition;
            break;
        default:
            break;
        }

        UpdateOverlapped();
    }
    usize Read(u8 *pBytes, usize nSize) override
    {
        we_assert(IsValid());

        if (nSize == 0)
        {
            return 0;
        }

        DWORD bytesRead = 0;
        do
        {
            const DWORD sizeClamped = CMath::Min((DWORD)nSize, (DWORD)MAXDWORD);
            DWORD read = 0;

            if (::ReadFile(mHandle, pBytes, (DWORD)nSize, &read, &mOverlapped) == FALSE)
            {
                DWORD error = GetLastError();
                if (error != ERROR_IO_PENDING)
                {
                    return 0;
                }

                read = 0;
                if (GetOverlappedResult(mHandle, &mOverlapped, (::DWORD *)&read, TRUE) == FALSE)
                {
                    error = GetLastError();
                    return 0;
                }
            }

            nSize -= sizeClamped;
            pBytes += sizeClamped;
            bytesRead += read;
            mPosition += read;
            UpdateOverlapped();

            if (sizeClamped != read)
            {
                return 0;
            }
        } while (nSize > 0);

        return static_cast<size_t>(bytesRead);
    }
    usize Write(const u8 *pBytes, usize nSize) override
    {
        we_assert(IsValid());

        if (nSize <= 0)
        {
            return 0;
        }

        DWORD bytesWritten = 0;

        if (::WriteFile(mHandle, pBytes, static_cast<DWORD>(nSize), &bytesWritten, &mOverlapped) == FALSE)
        {
            auto lastError = GetLastError();
            if (lastError != ERROR_IO_PENDING)
            {
                return 0;
            }

            bytesWritten = 0;
            if (GetOverlappedResult(mHandle, &mOverlapped, &bytesWritten, TRUE) == 0)
            {
                lastError = GetLastError();
                return 0;
            }
        }

        mPosition += bytesWritten;

        UpdateOverlapped();

        mSize = (mPosition > mSize) ? mPosition : mSize;

        return (usize)bytesWritten;
    }

  private:
    CWindowsFileNative(HANDLE handle, CPath filename, bool bCanRead, bool bCanWrite)
        : mHandle(handle), mCanRead(bCanRead), mCanWrite(bCanWrite), mFilename(std::move(filename))
    {
        if (IsValid())
        {
            UpdateFileStats();
        }
    }

    void UpdateFileStats()
    {
        LARGE_INTEGER largeInt;
        ::GetFileSizeEx(mHandle, &largeInt);
        mSize = largeInt.QuadPart;

        /*FILETIME creation_time{};
        FILETIME last_modified{};

        if (::GetFileTime(mHandle, &creation_time, nullptr, &last_modified))
        {
            mCreationTime = WindowsToDateTime(&creation_time);
            mLastModificationTime = WindowsToDateTime(&last_modified);
        }*/
    }

    void UpdateOverlapped()
    {
        const ULARGE_INTEGER largeInt{.QuadPart = mPosition};
        mOverlapped.Offset = largeInt.LowPart;
        mOverlapped.OffsetHigh = largeInt.HighPart;
    }

    HANDLE mHandle{nullptr};
    OVERLAPPED mOverlapped{0};
    usize mPosition{0};
    usize mSize{0};
    bool mCanRead;
    bool mCanWrite;
    const CPath mFilename;
};

IFileNative *CWindowsFileSystem::OpenRead(const CPath &filename, bool bCanWrite)
{
    we_assert(!filename.IsEmpty() && "Invalid filename");

    const DWORD access = GENERIC_READ;
    const DWORD shareMode = FILE_SHARE_READ | (bCanWrite ? FILE_SHARE_WRITE : 0);
    const DWORD creationFlag = OPEN_EXISTING;
    const DWORD flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;

    HANDLE handle = CreateFileW(*filename, access, shareMode, nullptr, creationFlag, flags, nullptr);
    if (handle == nullptr or handle == INVALID_HANDLE_VALUE)
    {
        // TODO: Log some error
        return nullptr;
    }

    return we_new(CWindowsFileNative, handle, filename, true, bCanWrite);
}

IFileNative *CWindowsFileSystem::OpenWrite(const CPath &filename, bool bAppend, bool bCanRead)
{
    const DWORD access = GENERIC_WRITE | (bCanRead ? GENERIC_READ : 0);
    const DWORD shareMode = bCanRead ? FILE_SHARE_READ : 0;
    const DWORD creationFlag = bAppend ? OPEN_ALWAYS : CREATE_ALWAYS;
    const DWORD flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;

    HANDLE handle = CreateFileW(*filename, access, shareMode, nullptr, creationFlag, flags, nullptr);
    if (handle == nullptr or handle == INVALID_HANDLE_VALUE)
    {
        // TODO: Log some error
        return nullptr;
    }

    CWindowsFileNative *result = we_new(CWindowsFileNative, handle, filename, bCanRead, true);

    if (bAppend)
    {
        result->Seek(0, SEEK_END);
    }

    return result;
}

TArray<CPath> CWindowsFileSystem::ListChildren(const CPath &path) const
{
    TArray<CPath> result;

    HANDLE hFind = INVALID_HANDLE_VALUE;
    const CPath searchPath = path + WT("/*");
    WIN32_FIND_DATA ffd = {0};

    hFind = ::FindFirstFile(*searchPath, &ffd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (::FindNextFile(hFind, &ffd) != 0)
        {
            if (ffd.cFileName[0] != '.' and ffd.cFileName[1] != '\0' and ffd.cFileName[1] != '.' and ffd.cFileName[2] != '\0')
            {
                result.Add(path / ffd.cFileName);
            }
        }
    }

    FindClose(hFind);

    return result;
}

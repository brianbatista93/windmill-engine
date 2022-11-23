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

bool CWindowsFileSystem::Initialize()
{
    wchar_t appPathWide[WE_OS_MAX_PATH]{0};
    usize length = ::GetModuleFileNameW(nullptr, appPathWide, WE_OS_MAX_PATH);
    if (length == 0)
    {
        return false;
    }
    CPath appFilePath({appPathWide, length});
    CPath appPath = appFilePath.GetParentPath();
    m_MountedDirs[(u32)EResourceMountType::eAssets] = appPath / WTL("Assets");
    m_MountedDirs[(u32)EResourceMountType::eDebug] = appPath / WTL("Logs");

    PWSTR userDocumentsWide = nullptr;
    HRESULT result = ::SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &userDocumentsWide);
    if (FAILED(result))
    {
        return false;
    }
    m_MountedDirs[(u32)EResourceMountType::eHome] = CPath({userDocumentsWide, ::wcslen(userDocumentsWide)});
    CoTaskMemFree(userDocumentsWide);

    return true;
}

void CWindowsFileSystem::Shutdown()
{
}

bool CWindowsFileSystem::FileExists(const CPath &path)
{
    DWORD attributes = ::GetFileAttributesW(*path);
    return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool CWindowsFileSystem::DirectoryExists(const CPath &path)
{
    const DWORD attributes = ::GetFileAttributesW(*path);
    return (attributes != INVALID_FILE_ATTRIBUTES) and (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool CWindowsFileSystem::CreateDirectory(const CPath &path)
{
    if (DirectoryExists(path))
    {
        return true;
    }

    CPath parentPath = path.GetParentPath();
    if (!parentPath.IsEmpty() and !DirectoryExists(parentPath))
    {
        CreateDirectory(parentPath);
    }

    return ::CreateDirectoryW(*path, nullptr) ? true : false;
}

class CWindowsFileNative : public IFileNative
{
    friend CMemoryManager;
    friend CWindowsFileSystem;

  public:
    virtual ~CWindowsFileNative() { Close(); }

    bool IsValid() const override { return m_Handle != NULL and m_Handle != INVALID_HANDLE_VALUE; }
    usize GetSize() const override { return m_Size; }
    void Close() override
    {
        if (IsValid())
        {
            Flush();
            if (::CloseHandle(m_Handle) == TRUE)
            {
                m_Handle = NULL;
            }
        }
        m_bCanRead = false;
        m_bCanWrite = false;
        m_Size = 0;
        m_Position = 0;
    }
    void Flush() override { ::FlushFileBuffers(m_Handle); }
    bool CanRead() const override { return m_bCanRead; }
    bool CanWrite() const override { return m_bCanWrite; }
    void Seek(usize nPosition, i32 mode) override
    {
        we_assert(IsValid());

        switch (mode)
        {
        case SEEK_SET:
            we_assert(nPosition >= 0);
            m_Position = nPosition;
            break;
        case SEEK_CUR:
            m_Position += nPosition;
            break;
        case SEEK_END:
            we_assert(nPosition <= 0);
            m_Position = m_Size + nPosition;
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

        DWORD bytes_read = 0;
        do
        {
            DWORD size_clamped = CMath::Min((DWORD)nSize, (DWORD)MAXDWORD);
            DWORD read = 0;

            if (::ReadFile(m_Handle, pBytes, (DWORD)nSize, &read, &m_Overlapped) == FALSE)
            {
                DWORD error = GetLastError();
                if (error != ERROR_IO_PENDING)
                {
                    return 0;
                }

                read = 0;
                if (GetOverlappedResult(m_Handle, &m_Overlapped, (::DWORD *)&read, TRUE) == FALSE)
                {
                    error = GetLastError();
                    return 0;
                }
            }

            nSize -= size_clamped;
            pBytes += size_clamped;
            bytes_read += read;
            m_Position += read;
            UpdateOverlapped();

            if (size_clamped != read)
            {
                return 0;
            }
        } while (nSize > 0);

        return static_cast<size_t>(bytes_read);
    }
    usize Write(const u8 *pBytes, usize nSize) override
    {
        we_assert(IsValid());

        if (nSize <= 0)
        {
            return 0;
        }

        DWORD BytesWritten = 0;

        if (::WriteFile(m_Handle, pBytes, static_cast<DWORD>(nSize), &BytesWritten, &m_Overlapped) == FALSE)
        {
            auto Err = GetLastError();
            if (Err != ERROR_IO_PENDING)
            {
                return 0;
            }

            BytesWritten = 0;
            if (GetOverlappedResult(m_Handle, &m_Overlapped, &BytesWritten, TRUE) == 0)
            {
                Err = GetLastError();
                return 0;
            }
        }

        m_Position += BytesWritten;

        UpdateOverlapped();

        m_Size = (m_Position > m_Size) ? m_Position : m_Size;

        return (usize)BytesWritten;
    }

  private:
    CWindowsFileNative(HANDLE handle, const CPath &filename, bool bCanRead, bool bCanWrite)
        : m_Handle(handle), m_Position(0), m_Size(0), m_bCanRead(bCanRead), m_bCanWrite(bCanWrite), m_Filename(filename)
    {
        if (IsValid())
        {
            UpdateFileStats();
        }
    }

    void UpdateFileStats()
    {
        LARGE_INTEGER large_int;
        ::GetFileSizeEx(m_Handle, &large_int);
        m_Size = large_int.QuadPart;

        /*FILETIME creation_time{};
        FILETIME last_modified{};

        if (::GetFileTime(m_Handle, &creation_time, NULL, &last_modified))
        {
            m_CreationTime = WindowsToDateTime(&creation_time);
            m_LastModificationTime = WindowsToDateTime(&last_modified);
        }*/
    }

    void UpdateOverlapped()
    {
        ULARGE_INTEGER large_int{.QuadPart = m_Position};
        m_Overlapped.Offset = large_int.LowPart;
        m_Overlapped.OffsetHigh = large_int.HighPart;
    }

    HANDLE m_Handle{NULL};
    OVERLAPPED m_Overlapped{0};
    usize m_Position;
    usize m_Size;
    bool m_bCanRead;
    bool m_bCanWrite;
    CPath m_Filename;
};

IFileNative *CWindowsFileSystem::OpenRead(const CPath &filename, bool bCanWrite)
{
    we_assert(!filename.IsEmpty() && "Invalid filename");

    const DWORD access = GENERIC_READ;
    const DWORD shareMode = FILE_SHARE_READ | (bCanWrite ? FILE_SHARE_WRITE : 0);
    const DWORD creationFlag = OPEN_EXISTING;
    const DWORD flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;

    HANDLE handle = CreateFileW(*filename, access, shareMode, NULL, creationFlag, flags, NULL);
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

    HANDLE handle = CreateFileW(*filename, access, shareMode, NULL, creationFlag, flags, NULL);
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

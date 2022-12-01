#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include "HAL/FileSystem.hpp"
#include "LinuxFileSystem.hpp"
#include "OS/StringConvertion.hpp"

template <class To, class From>
class TStringCast
{
};

template <>
class TStringCast<ansi, wide>
{
  public:
    inline TStringCast(const wide *pSrc, usize nLength) { OS::TCharToUTF8((utf8 *)m_pBuffer, pSrc, nLength); }

    inline const ansi *operator*() const { return m_pBuffer; }

  private:
    ansi m_pBuffer[1024];
};

template <>
class TStringCast<wide, ansi>
{
  public:
    inline TStringCast(const ansi *pSrc, usize nLength) { OS::UTF8ToTChar(m_pBuffer, (const utf8 *)pSrc, nLength); }

    inline const wide *operator*() const { return m_pBuffer; }

  private:
    wide m_pBuffer[1024];
};

IFileSystem *CFileSystem::Get()
{
    static CLinuxFileSystem sInstance;
    return &sInstance;
}

bool CLinuxFileSystem::Initialize()
{
    ansi appFilePathAnsi[WE_OS_MAX_PATH]{0};
    const usize length = readlink("/proc/self/exe", appFilePathAnsi, WE_OS_MAX_PATH);
    CPath appFilePath(CString(*TStringCast<wide, ansi>(appFilePathAnsi, length), length));
    CPath appPath = appFilePath.GetParentPath();
    m_MountedDirs[(u32)EResourceMountType::eAssets] = appPath / WTL("Assets");
    m_MountedDirs[(u32)EResourceMountType::eDebug] = appPath / WTL("Logs");

    const ansi *homeDir = nullptr;
    if ((homeDir = getenv("HOME")) == nullptr)
    {
        homeDir = getpwuid(getuid())->pw_dir;
    }
    m_MountedDirs[(u32)EResourceMountType::eHome] = CPath(CString(*TStringCast<wide, ansi>(homeDir, strlen(homeDir)), length));

    return true;
}

void CLinuxFileSystem::Shutdown()
{
}

bool CLinuxFileSystem::FileExists(const CPath &path)
{
    struct stat s;
    if (stat(*TStringCast<ansi, wide>(*path, path.GetLength()), &s))
    {
        return false;
    }

    return S_ISREG(s.st_mode);
}

bool CLinuxFileSystem::DirectoryExists(const CPath &path)
{
    struct stat s;
    if (stat(*TStringCast<ansi, wide>(*path, path.GetLength()), &s))
    {
        return false;
    }

    return (s.st_mode & S_IFDIR) != 0;
}

bool CLinuxFileSystem::CreateDirectory(const CPath &path)
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

    return mkdir(*TStringCast<ansi, wide>(*path, path.GetLength()), 0777) == 0;
}

class CLinuxFile : public IFileNative
{
    friend CMemoryManager;
    friend CLinuxFileSystem;

  public:
    ~CLinuxFile() { Close(); }

    bool IsValid() const override { return m_Handle != -1; }
    usize GetSize() const override { return m_Size; }
    void Close() override
    {
        close(m_Handle);
        m_bCanRead = false;
        m_bCanWrite = false;
    }
    void Flush() override { fsync(m_Handle); }
    bool CanRead() const override { return m_bCanRead; }
    bool CanWrite() const override { return m_bCanWrite; }
    void Seek(usize nPosition, i32 nMode) override
    {
        we_assert(IsValid());

        lseek(m_Handle, nPosition, nMode);
    }
    usize Read(u8 *pBytes, usize nSize) override
    {
        we_assert(IsValid());
        we_assert(CanRead());

        i64 bytesRead = 0;
        while (nSize)
        {
            bytesRead += read(m_Handle, pBytes, nSize);
            if (bytesRead != -1 or errno != EFAULT)
            {
                if (bytesRead != i64(nSize))
                {
                    return bytesRead;
                }
                pBytes += nSize;
                nSize -= bytesRead;
            }
        }

        return bytesRead;
    }
    usize Write(const u8 *pBytes, usize nSize) override
    {
        we_assert(IsValid());
        we_assert(CanWrite());

        if (write(m_Handle, pBytes, nSize) != ssize_t(nSize))
        {
            return 0;
        }

        pBytes += nSize;

        return nSize;
    }

  private:
    CLinuxFile(i32 handle, const CPath &filename, bool bCanRead, bool bCanWrite)
        : m_Handle(handle), m_Size(0), m_bCanRead(bCanRead), m_bCanWrite(bCanWrite), m_Filename(filename)
    {
        struct stat fileStat;
        if (fstat(handle, &fileStat) == 0)
        {
            m_Size = fileStat.st_size;
        }
    }

    i32 m_Handle;
    usize m_Size;
    bool m_bCanRead;
    bool m_bCanWrite;
    CPath m_Filename;
};

IFileNative *CLinuxFileSystem::OpenRead(const CPath &filename, bool bCanWrite)
{
    i32 flags = bCanWrite ? O_RDWR : O_RDONLY;
    i32 handle = open(*TStringCast<ansi, wide>(*filename, filename.GetLength()), flags);
    if (handle == -1)
    {
        return nullptr;
    }

    return we_new(CLinuxFile, handle, filename, true, bCanWrite);
}

IFileNative *CLinuxFileSystem::OpenWrite(const CPath &filename, bool bAppend, bool bCanRead)
{
    i32 flags = bCanRead ? O_RDWR : O_WRONLY;
    flags |= bAppend ? O_APPEND : O_CREAT;
    i32 handle = open(*TStringCast<ansi, wide>(*filename, filename.GetLength()), flags);
    if (handle == -1)
    {
        return nullptr;
    }

    return we_new(CLinuxFile, handle, filename, bCanRead, true);
}

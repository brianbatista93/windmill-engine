#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include "HAL/FileSystem.hpp"
#include "LinuxFileSystem.hpp"
#include "OS/StringConvertion.hpp"

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
    mMountedDirs[(u32)EResourceMountType::eAssets] = appPath / WTL("Assets");
    mMountedDirs[(u32)EResourceMountType::eDebug] = appPath / WTL("Logs");

    const ansi *homeDir = nullptr;
    if ((homeDir = getenv("HOME")) == nullptr)
    {
        homeDir = getpwuid(getuid())->pw_dir;
    }
    mMountedDirs[(u32)EResourceMountType::eHome] = CPath(CString(*TStringCast<wide, ansi>(homeDir, strlen(homeDir)), length));

    return true;
}

void CLinuxFileSystem::Shutdown()
{
}

bool CLinuxFileSystem::FileExists(const CPath &path) const
{
    struct stat s;
    if (stat(*TStringCast<ansi, wide>(*path, path.GetLength()), &s))
    {
        return false;
    }

    return S_ISREG(s.st_mode);
}

bool CLinuxFileSystem::DirectoryExists(const CPath &path) const
{
    struct stat s;
    if (stat(*TStringCast<ansi, wide>(*path, path.GetLength()), &s))
    {
        return false;
    }

    return S_ISDIR(s.st_mode);
}

bool CLinuxFileSystem::CreateDirectory(const CPath &path) const
{
    if (DirectoryExists(path))
    {
        return true;
    }

    TArray<CString> pathNodes = path.ToString().Split(CPath::kSeparator);

    CPath directory;
    for (auto &node : pathNodes)
    {
        directory = directory / *node;
        if (mkdir(*TStringCast<ansi, wide>(*directory, directory.GetLength()), 0775) == -1)
        {
            i32 error = errno;
            if (error == EEXIST)
            {
                continue;
            }

            return false;
        }
    }

    return true;
}

class CLinuxFile : public IFileNative
{
    friend CMemoryManager;
    friend CLinuxFileSystem;

  public:
    ~CLinuxFile() { Close(); }

    bool IsValid() const override { return mHandle != -1; }
    usize GetSize() const override { return m_Size; }
    void Close() override
    {
        close(mHandle);
        mCanRead = false;
        mCanWrite = false;
    }
    void Flush() override { fsync(mHandle); }
    bool CanRead() const override { return mCanRead; }
    bool CanWrite() const override { return mCanWrite; }
    void Seek(usize nPosition, i32 nMode) override
    {
        we_assert(IsValid());

        lseek(mHandle, nPosition, nMode);
    }
    usize Read(u8 *pBytes, usize nSize) override
    {
        we_assert(IsValid());
        we_assert(CanRead());

        i64 bytesRead = 0;
        while (nSize)
        {
            bytesRead += read(mHandle, pBytes, nSize);
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

        if (write(mHandle, pBytes, nSize) != ssize_t(nSize))
        {
            return 0;
        }

        pBytes += nSize;

        return nSize;
    }

  private:
    CLinuxFile(i32 handle, CPath filename, bool bCanRead, bool bCanWrite)
        : mHandle(handle), mCanRead(bCanRead), mCanWrite(bCanWrite), mFilename(std::move(filename))
    {
        struct stat fileStat;
        if (fstat(handle, &fileStat) == 0)
        {
            m_Size = fileStat.st_size;
        }
    }

    i32 mHandle;
    usize m_Size{0};
    bool mCanRead;
    bool mCanWrite;
    const CPath mFilename;
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
    i32 flags = O_CREAT | O_CLOEXEC;

    if (bCanRead)
    {
        flags |= O_RDWR;
    }
    else
    {
        flags |= O_WRONLY;
    }

    i32 handle = open(*TStringCast<ansi, wide>(*filename, filename.GetLength()), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (handle == -1)
    {
        printf("Error: %s\n", std::strerror(errno));
        return nullptr;
    }

    auto result = we_new(CLinuxFile, handle, filename, bCanRead, true);

    if (bAppend)
    {
        result->Seek(0, SEEK_END);
    }

    return result;
}

TArray<CPath> CLinuxFileSystem::ListChildren(const CPath &path) const
{
    TArray<CPath> result;

    DIR *dir = NULL;
    struct dirent *ent = NULL;

    if ((dir = opendir(*TStringCast<ansi, wide>(*path, path.GetLength()))) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.' and ent->d_name[1] != '\0' and ent->d_name[1] != '.' and ent->d_name[2] != '\0')
            {
                const usize len = strlen(ent->d_name);
                const CPath entPath = path / CString(*TStringCast<wide, ansi>(ent->d_name, len));
                result.Add(entPath);
            }
        }
        closedir(dir);
    }

    return result;
}

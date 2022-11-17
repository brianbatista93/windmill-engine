#include "shlobj.h"
#include <WinBase.h>

#include "HAL/FileSystem.hpp"
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

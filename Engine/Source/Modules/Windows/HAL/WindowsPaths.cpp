#include "HAL/Path.hpp"
#include "WindowsInclude.hpp"

CPath CPaths::kCachedWorkingDirectory;

CPath CPaths::GetWorkingDirectory()
{
    if UNLIKELY (kCachedWorkingDirectory.IsEmpty())
    {
        tchar pWorkingDir[WE_OS_MAX_PATH];
        ::GetCurrentDirectory(WE_OS_MAX_PATH, pWorkingDir);
        kCachedWorkingDirectory = pWorkingDir;
    }

    return kCachedWorkingDirectory;
}

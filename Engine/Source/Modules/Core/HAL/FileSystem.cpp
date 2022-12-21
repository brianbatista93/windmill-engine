#include "FileSystem.hpp"

struct SMappedDirectoryInfo
{
    const IFileSystem *pFileSystem = nullptr;
    CPath Path;
    EResourceMountType eMountType = EResourceMountType::eAssets;
};

static SMappedDirectoryInfo sMappedDirectories[(i32)EResourceType::eCount]{};

bool CFileSystem::MatchFilter(const CPath &path, const tchar *pFilter)
{
    we_assert(pFilter and *pFilter);

    if (pFilter[0] == WT('*'))
    {
        if (pFilter[1] == 0)
        {
            return true;
        }

        CString filterEnd(pFilter + 1);

        if (path.GetLength() < filterEnd.GetLength())
        {
            return false;
        }

        const CString pathString = path.ToString();

        return pathString.EndsWith(*filterEnd);
    }

    return false;
}

bool CFileSystem::MapDirectory(EResourceMountType eResourceMountType, EResourceType eResourceType, const CPath &path)
{
    if (path.IsEmpty())
    {
        return false;
    }

    auto &info = sMappedDirectories[(i32)eResourceType];

    info.eMountType = eResourceMountType;
    info.Path = Get()->mMountedDirs[(i32)eResourceMountType] / path;

    if (eResourceMountType != EResourceMountType::eAssets and !CFileSystem::DirectoryExists(info.Path))
    {
        if (!CFileSystem::CreateDirectory(info.Path))
        {
            return false;
        }
    }

    return true;
}

#include "Path.hpp"
#include "FileSystem.hpp"

CPath &CPath::Append(const tchar *pStr, i32 nLength)
{
    i32 dataCount = mText.mData.GetSize();
    if (nLength == 0)
    {
        if (dataCount > 1 and mText.mData[dataCount - 2] != WT('/') and mText.mData[dataCount - 2] != WT('\\'))
        {
            mText.mData[dataCount - 1] = WT('/');
            mText.mData.Add(WT('\0'));
        }
    }
    else
    {
        if (dataCount > 0)
        {
            if (dataCount > 1 and mText.mData[dataCount - 2] != WT('/') and mText.mData[dataCount - 2] != WT('\\') and *pStr != WT('/'))
            {
                mText.mData[dataCount - 1] = WT('/');
            }
            else
            {
                mText.mData.Pop();
                --dataCount;
            }
        }

        mText.mData.Reserve(dataCount + nLength);
        mText.mData.Append(pStr, nLength);
        mText.mData.Add(WT('\0'));
    }

    return *this;
}

bool CPath::IsDirectory() const
{
    return CFileSystem::DirectoryExists(*this) and !CFileSystem::FileExists(*this);
}

bool CPath::IsFile() const
{
    return CFileSystem::FileExists(*this) and !CFileSystem::DirectoryExists(*this);
}

CArray<CPath> GetAllFilesImpl(const CPath &path)
{
    return CFileSystem::ListChildren(path);
}

CArray<CPath> CPath::GetAllFiles(const tchar *pFilter, bool bRecursive) const
{
    we_assert(!IsEmpty());
    we_assert(IsDirectory());
    we_assert(pFilter and *pFilter);

    CArray<CPath> result;

    const CArray<CPath> files = GetAllFilesImpl(*this);

    for (const auto &path : files)
    {
        if (path.IsFile() and CFileSystem::MatchFilter(path, pFilter))
        {
            result.Add(path);
        }
        else if (bRecursive and path.IsDirectory())
        {
            result.Append(path.GetAllFiles(pFilter, bRecursive));
        }
    }

    return result;
}

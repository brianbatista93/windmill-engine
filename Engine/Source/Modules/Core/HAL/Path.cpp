#include "Path.hpp"
#include "FileSystem.hpp"

CPath &CPath::Append(const tchar *pStr, i32 nLength)
{
    i32 dataCount = mText.m_Data.GetSize();
    if (nLength == 0)
    {
        if (dataCount > 1 and mText.m_Data[dataCount - 2] != WT('/') and mText.m_Data[dataCount - 2] != WT('\\'))
        {
            mText.m_Data[dataCount - 1] = WT('/');
            mText.m_Data.Add(WT('\0'));
        }
    }
    else
    {
        if (dataCount > 0)
        {
            if (dataCount > 1 and mText.m_Data[dataCount - 2] != WT('/') and mText.m_Data[dataCount - 2] != WT('\\') and *pStr != WT('/'))
            {
                mText.m_Data[dataCount - 1] = WT('/');
            }
            else
            {
                mText.m_Data.Pop();
                --dataCount;
            }
        }

        mText.m_Data.Reserve(dataCount + nLength);
        mText.m_Data.Append(pStr, nLength);
        mText.m_Data.Add(WT('\0'));
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

TArray<CPath> GetAllFilesImpl(const CPath &path)
{
    return CFileSystem::ListChildren(path);
}

TArray<CPath> CPath::GetAllFiles(const tchar *pFilter, bool bRecursive) const
{
    we_assert(!IsEmpty());
    we_assert(IsDirectory());
    we_assert(pFilter and *pFilter);

    TArray<CPath> result;

    const TArray<CPath> files = GetAllFilesImpl(*this);

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

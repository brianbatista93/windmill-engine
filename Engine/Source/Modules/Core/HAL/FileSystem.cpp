#include "FileSystem.hpp"

bool CFileSystem::MatchFilter(const CPath &path, const tchar *pFilter)
{
    we_assert(pFilter and *pFilter);

    if (pFilter[0] == WT('*'))
    {
        if (pFilter[1] == 0)
        {
            return true;
        }

        const CString filterEnd(pFilter + 1);

        if (path.GetLength() < filterEnd.GetLength())
        {
            return false;
        }

        const CString pathString = path.ToString();

        return pathString.EndsWith(*filterEnd);
    }

    return false;
}

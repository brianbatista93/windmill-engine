#include <cerrno>

#include "HAL/File.hpp"
#include "Logging/Log.hpp"
#include "OS/OSEntry.hpp"

DECLARE_STATIC_LOG_EMITTER(Scribe, eInfo);

extern bool ProcessFile(const CPath &filePath);

MAIN_ENTRY
{
    if (!CLogSystem::Initialize())
    {
        return ENOTTY;
    }

    if (nArgC == 1)
    {
        WE_ERROR(Scribe, WT("No input file specified"));
        return EINVAL;
    }

    const CPath filePath(ppArgV[1]);

    WE_INFO(Scribe, WT("Precomputing file: {0}"), *filePath);

    if (!CFile::Exist(CPath(ppArgV[1])))
    {
        WE_ERROR(Scribe, WT("File {0} does not exist"), *filePath);
        return ENOENT;
    }

    if (!ProcessFile(filePath))
    {
        return -1;
    }

    CLogSystem::Shutdown();

    return 0;
}
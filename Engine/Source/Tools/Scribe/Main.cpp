#include <cerrno>

#include "Containers/Array.hpp"
#include "HAL/File.hpp"
#include "HAL/FileSystem.hpp"
#include "Logging/Log.hpp"
#include "Logging/LogFileSink.hpp"
#include "Logging/StdLogSink.hpp"
#include "OS/OSEntry.hpp"

DECLARE_STATIC_LOG_EMITTER(Scribe, eInfo);

extern bool ProcessFile(const CPath &filePath, const CPath &output);
extern CPath gLicenseFile;

i32 InitScribe(i32 nArgC, tchar *ppArgV[])
{
    if (nArgC == 1)
    {
        WE_ERROR(Scribe, WT("No input specified"));
        return EINVAL;
    }

    CPath outputDirectory;

    for (i32 i = 1; i < nArgC; ++i)
    {
        if (ppArgV[i][0] == WT('-'))
        {
            if (ppArgV[i][1] == WT('o') and ppArgV[i][2] == 0)
            {
                outputDirectory = CString(ppArgV[++i]);
            }
            else if (ppArgV[i][1] == WT('l') and ppArgV[i][2] == 0)
            {
                gLicenseFile = CString(ppArgV[++i]);
            }
        }
    }

    if (outputDirectory.IsEmpty())
    {
        WE_ERROR(Scribe, WT("No output directory specified"));
        return EINVAL;
    }

    const CPath filePath(ppArgV[1]);

    TArray<CPath> files;

    if (filePath.IsDirectory())
    {
        const TArray<CPath> headerFiles = filePath.GetAllFiles(WT("*.hpp"), true);
        for (auto file : headerFiles)
        {
            files.Add(file);
        }
    }
    else if (filePath.IsFile())
    {
        files.Add(filePath);
    }

    for (auto file : files)
    {
        if (!ProcessFile(file, outputDirectory))
        {
            return -1;
        }
    }

    return 0;
}

MAIN_ENTRY_BEGIN
{
    if (!CLogSystem::Initialize())
    {
        return ENOTTY;
    }

    if (!CLogSystem::Get().AddSink(we_new(CLogFileSink, CPath(WT("./Scribe.log")))))
    {
        errorCode = -1;
    }

    if (!CLogSystem::Get().AddSink(we_new(CStdLogSink)))
    {
        errorCode = -1;
    }

    if (errorCode == 0)
    {
        errorCode = InitScribe(nArgC, ppArgV);
    }

    CLogSystem::Shutdown();
}
MAIN_ENTRY_END
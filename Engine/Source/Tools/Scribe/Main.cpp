#include <cerrno>
#include <iostream>

#include "Containers/Array.hpp"
#include "HAL/File.hpp"
#include "HAL/FileSystem.hpp"
#include "Logging/Log.hpp"
#include "Logging/LogFileSink.hpp"
#include "Logging/StdLogSink.hpp"
#include "OS/OSEntry.hpp"
#include "ScribeExceptions.hpp"

DECLARE_STATIC_LOG_EMITTER(Scribe, eInfo);

extern bool ProcessFile(const CPath &filePath, const CPath &output);
extern CPath gLicenseFile;

void InitScribe(i32 nArgC, tchar *ppArgV[])
{
    if (nArgC == 1)
    {
        throw CScribeException("No input file specified");
    }

    CPath outputDirectory = {};

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
        throw CScribeException("No output directory specified");
    }

    const CPath filePath{ppArgV[1]};

    TArray<CPath> files = {};

    if (filePath.IsDirectory())
    {
        const TArray<CPath> headerFiles{filePath.GetAllFiles(WT("*.hpp"), true)};
        for (auto &file : headerFiles)
        {
            files.Add(file);
        }
    }
    else if (filePath.IsFile())
    {
        files.Add(filePath);
    }

    for (auto &file : files)
    {
        if (!ProcessFile(file, outputDirectory))
        {
            throw CScribeException("Failed to process file");
        }
    }
}

MAIN_ENTRY_BEGIN
{

    class CLogRAII
    {
      public:
        ~CLogRAII() { CLogSystem::Shutdown(); }
    };

    if (!CLogSystem::Initialize())
    {
        return ENOTTY;
    }

    if (!CLogSystem::Get().AddSink(we_new(CLogFileSink, CPath(WT("./Scribe.log")))) or !CLogSystem::Get().AddSink(we_new(CStdLogSink)))
    {
        return -1;
    }

    CLogRAII logRAII;

    try
    {
        InitScribe(nArgC, ppArgV);
    }
    catch (CScribeException &e)
    {
        WE_ERROR(Scribe, WT("%s"), (const tchar *)e.what());
        return -1;
    }
    catch (std::exception &e)
    {
        std::cerr << "Unhandled exception : " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
MAIN_ENTRY_END

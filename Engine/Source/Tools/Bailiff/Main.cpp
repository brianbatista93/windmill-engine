#include <cerrno>
#include <iostream>

#include "Containers/Array.hpp"
#include "Exceptions/BailiffException.hpp"
#include "HAL/File.hpp"
#include "HAL/FileSystem.hpp"
#include "Logging/Log.hpp"
#include "Logging/LogFileSink.hpp"
#include "Logging/OSLogSink.hpp"
#include "Logging/StdLogSink.hpp"
#include "OS/OSEntry.hpp"
#include "Project/BailiffProject.hpp"

DECLARE_STATIC_LOG_EMITTER(Bailiff, eAll);

MAIN_ENTRY_BEGIN
{
    class CLogRAII
    {
      public:
        CLogRAII() { CLogSystem::Initialize(); }
        ~CLogRAII() { CLogSystem::Shutdown(); }
    } logRAII;

    if (!CLogSystem::Get().AddSinks(we_new(CStdLogSink)))
    {
        return -1;
    }

    try
    {
        if (CStringUtils::Compare(ppArgV[1], WT("project")) == 0)
        {
            CBailiffProject::Create(ppArgV, nArgC);
        }
    }
    catch (const CBailiffException &exc)
    {
        WE_ERROR(Bailiff, WT("{0}"), exc.GetWhat());
        return -1;
    }
    catch (const std::exception &exc)
    {
        WE_ERROR(Bailiff, WT("{0}"), exc.what());
        return -1;
    }

    return 0;
}
MAIN_ENTRY_END

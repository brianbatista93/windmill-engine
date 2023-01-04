#include "Engine.hpp"
#include "Containers/ArrayView.hpp"
#include "HAL/FileSystem.hpp"
#include "Logging/Log.hpp"
#include "Logging/LogFileSink.hpp"
#include "Logging/StdLogSink.hpp"
#include "OS/OS.hpp"

DECLARE_STATIC_LOG_EMITTER(Engine, eInfo);

CEngine &CEngine::Get()
{
    static CEngine sInstance;
    return sInstance;
}

bool CEngine::Initialize(const CArrayView<tchar *> &arguments)
{
    if (!ProcessArguments(arguments))
    {
        return false;
    }

    if (!CLogSystem::Initialize())
    {
        return false;
    }
    CLogSystem::Get().AddSink(we_new(CStdLogSink));
    CLogSystem::Get().AddSink(we_new(CLogFileSink, WTL("Engine.log")));

    if (!OS::Initialize())
    {
        return false;
    }

    if (!mConfigFile.Load(WTL("../Config/Engine.ini")))
    {
        return false;
    }

    return true;
}

void CEngine::Shutdown()
{
    mConfigFile.Save();
    OS::Shutdown();
    CLogSystem::Shutdown();
}

void CEngine::Tick()
{
}

// NOLINTBEGIN
bool CEngine::ProcessArguments(const CArrayView<tchar *> & /*arguments*/)
{
    return true;
}
// NOLINTEND

bool EngineInitialize(const CArrayView<tchar *> &arguments)
{
    return CEngine::Get().Initialize(arguments);
}

void EngineShutdown()
{
}

void EngineLoop()
{
}

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

bool CEngine::Initialize(const TArrayView<tchar *> &arguments)
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

    return false;
}

void CEngine::Shutdown()
{
    OS::Shutdown();
    CLogSystem::Shutdown();
}

void CEngine::Tick()
{
}

bool CEngine::ProcessArguments(const TArrayView<tchar *> & /*arguments*/)
{
    return true;
}

bool EngineInitialize(const TArrayView<tchar *> &arguments)
{
    return CEngine::Get().Initialize(arguments);
}

void EngineShutdown()
{
}

void EngineLoop()
{
}

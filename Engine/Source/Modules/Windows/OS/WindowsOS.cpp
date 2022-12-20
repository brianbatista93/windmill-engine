#include "Logging/Log.hpp"
#include "Logging/WindowsOutputLogSink.hpp"
#include "OS/OS.hpp"

#include "WindowsInclude.hpp"

DECLARE_STATIC_LOG_EMITTER(Windows, eInfo);

namespace OS
{
bool Initialize()
{
    if (!CLogSystem::Get().AddSink(we_new(CWindowsOutputLogSink)))
    {
        return false;
    }

    WE_INFO(Windows, WT("Initializing Windows OS"));

    return true;
}

void Shutdown()
{
}
} // namespace OS
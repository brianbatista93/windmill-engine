#include "Logging/Log.hpp"
#include "Logging/WindowsOutputLogSink.hpp"
#include "OS/OS.hpp"

#include "WindowsInclude.hpp"

DECLARE_STATIC_LOG_EMITTER(Windows, eInfo);

namespace OS
{
bool Initialize()
{
    return !!CLogSystem::Get().AddSink(we_new(CWindowsOutputLogSink));
}

void Shutdown()
{
}
} // namespace OS

#include "Logging/Log.hpp"
#include "OS/OS.hpp"

DECLARE_STATIC_LOG_EMITTER(Linux, eInfo);

namespace OS
{
bool Initialize()
{
    return true;
}

void Shutdown()
{
}
} // namespace OS
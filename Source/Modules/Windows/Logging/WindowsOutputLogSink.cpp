#include "WindowsOutputLogSink.hpp"
#include "Logging/LogMessage.hpp"
#include "WindowsInclude.hpp"

void CWindowsOutputLogSink::Log(const SLogMessage *pMessage)
{
    ::OutputDebugString(*pMessage->FormattedMessage);
}
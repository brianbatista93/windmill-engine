#include "WindowsOutputLogSink.hpp"
#include "Logging/LogEmitter.hpp"
#include "Logging/LogMessage.hpp"
#include "WindowsInclude.hpp"

void CWindowsOutputLogSink::Log(const SLogMessage *pMessage)
{
    CStringBuilder builder;
    builder.AppendFormat(WT("{0:S} {1} --- "), ToString(pMessage->LogLevel), *pMessage->pEmitter->GetName());
    builder.AppendLine(pMessage->FormattedMessage);

    ::OutputDebugString(*builder.ToString());
}

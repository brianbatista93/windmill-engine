#include <iostream>

#include "LogEmitter.hpp"
#include "LogMessage.hpp"
#include "StdLogSink.hpp"

#ifdef UNICODE
    #define COUT std::wcout
    #define CERR std::wcerr
#else
    #define COUT std::cout
    #define CERR std::cerr
#endif // UNICODE

void CStdLogSink::Log(const SLogMessage *pMessage)
{
    CStringBuilder builder;
    builder.AppendFormat(WT("{0} [{1}] --- "), *pMessage->pEmitter->GetName(), ToString(pMessage->LogLevel));
    builder.AppendLine(pMessage->FormattedMessage);

    const CString message = builder.ToString();

    if (pMessage->LogLevel <= ELogLevel::eError)
    {
        CERR << *message << std::endl;
    }
    else
    {
        COUT << *message << std::endl;
    }
}
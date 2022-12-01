#include <iostream>

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
    if (pMessage->LogLevel <= ELogLevel::eError)
    {
        CERR << *pMessage->FormattedMessage << std::endl;
    }
    else
    {
        COUT << *pMessage->FormattedMessage << std::endl;
    }
}
/*
Copyright (C) 2021-2022 Bull Technology, Ltd.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Containers/ContainerFwd.hpp"

#include "Containers/Array.hpp"
#include "ILogSink.hpp"
#include "LogEmitter.hpp"
#include "LogLevel.hpp"
#include "LogMessage.hpp"
#include "Memory/Memory.hpp"

class CLogSystem
{
  public:
    static bool Initialize();
    static void Shutdown();

    bool AddSink(ILogSink *pSink);
    void RemoveSink(ILogSink *pSink);
    void RemoveAllSinks();

    template <class... Args>
    inline void Log(const CLogEmitter *pEmitter, ELogLevel logLevel, const char *pFile, i32 nLine, const char *pFunction, const tchar *pFormat,
                    Args &&...args)
    {
        if (pEmitter->ShouldLog(logLevel))
        {
            SLogMessage message = {};
            message.pEmitter = pEmitter;
            message.LogLevel = logLevel;
            message.pFilename = pFile;
            message.nLine = nLine;
            message.pFunction = pFunction;
            message.FormattedMessage = std::move(CString::Format(pFormat, std::forward<Args>(args)...));

            LogInternal(&message);
        }
    }

    static CLogSystem &Get();

  private:
    void LogInternal(const SLogMessage *pMessage);

    TArray<TUniquePtr<ILogSink>> m_Sinks;
};

#define DECLARE_EXTERN_LOG_EMITTER(emitter, logLevel) extern CLogEmitter _gLogEmitter##emitter(WT(#emitter), ELogLevel::logLevel) // NOLINT

#define DECLARE_STATIC_LOG_EMITTER(emitter, logLevel) static CLogEmitter _gLogEmitter##emitter(WT(#emitter), ELogLevel::logLevel) // NOLINT

#define WE_LOG(emitter, logLevel, pFormat, ...)                                                                                                      \
    CLogSystem::Get().Log(&_gLogEmitter##emitter, ELogLevel::logLevel, __FILE__, __LINE__, __FUNCTION__, pFormat, ##__VA_ARGS__)

#define WE_FATAL(emitter, pFormat, ...) WE_LOG(emitter, eFatal, pFormat, ##__VA_ARGS__)
#define WE_ERROR(emitter, pFormat, ...) WE_LOG(emitter, eError, pFormat, ##__VA_ARGS__)
#define WE_WARN(emitter, pFormat, ...) WE_LOG(emitter, eWarning, pFormat, ##__VA_ARGS__)
#define WE_INFO(emitter, pFormat, ...) WE_LOG(emitter, eInfo, pFormat, ##__VA_ARGS__)
#define WE_VERB(emitter, pFormat, ...) WE_LOG(emitter, eDebug, pFormat, ##__VA_ARGS__)
#define WE_TRACE(emitter, pFormat, ...) WE_LOG(emitter, eTrace, pFormat, ##__VA_ARGS__)

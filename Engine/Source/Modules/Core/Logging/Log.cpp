#include "Log.hpp"
#include "Globals.hpp"

CLogSystem *gLog = nullptr;

bool CLogSystem::Initialize()
{
    if (gLog == nullptr)
    {
        gLog = we_new(CLogSystem);
        return true;
    }

    return false;
}

void CLogSystem::Shutdown()
{
    if (gLog)
    {
        gLog->RemoveAllSinks();
        we_delete(gLog);
        gLog = nullptr;
    }
}

bool CLogSystem::AddSink(ILogSink *pSink)
{
    for (auto &existingSink : mSinks)
    {
        if (existingSink.get() == pSink)
        {
            return false;
        }
    }

    const i32 index = mSinks.AddSlots();
    mSinks[index].reset(pSink);
    return true;
}

void CLogSystem::RemoveSink(ILogSink *pSink)
{
    const auto *iter = std::find_if(mSinks.begin(), mSinks.end(), [pSink](TUniquePtr<ILogSink> &sink) { return sink.get() == pSink; });
    mSinks.RemoveAt((i32)(iter - mSinks.begin()));
}

void CLogSystem::RemoveAllSinks()
{
    mSinks.Clear();
}

CLogSystem &CLogSystem::Get()
{
    we_assert(gLog != nullptr && "Log system wasn't initialized");
    return *gLog;
}

void CLogSystem::LogInternal(const SLogMessage *pMessage)
{
    for (auto &sink : mSinks)
    {
        sink->Log(pMessage);
    }
}

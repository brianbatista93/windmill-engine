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
    for (auto &existingSink : m_Sinks)
    {
        if (existingSink.get() == pSink)
        {
            return false;
        }
    }

    i32 index = m_Sinks.AddSlots();
    m_Sinks[index].reset(pSink);
    return true;
}

void CLogSystem::RemoveSink(ILogSink *pSink)
{
    auto it = std::find_if(m_Sinks.begin(), m_Sinks.end(), [pSink](TUniquePtr<ILogSink> &a) { return a.get() == pSink; });
    m_Sinks.RemoveAt((i32)(it - m_Sinks.begin()));
}

void CLogSystem::RemoveAllSinks()
{
    m_Sinks.Clear();
}

CLogSystem &CLogSystem::Get()
{
    we_assert(gLog != nullptr && "Log system wasn't initialized");
    return *gLog;
}

void CLogSystem::LogInternal(const SLogMessage *pMessage)
{
    for (auto &sink : m_Sinks)
    {
        sink->Log(pMessage);
    }
}

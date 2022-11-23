#include "Engine.hpp"
#include "HAL/FileSystem.hpp"

#include "HAL/File.hpp"

CEngine &CEngine::Get()
{
    static CEngine sInstance;
    return sInstance;
}

bool CEngine::Initialize(TArray<tchar *, TAllocator<i32>> &&arguments)
{
    if (!ProcessArguments(std::move(arguments)))
    {
        return false;
    }

    if (!CFileSystem::Initialize())
    {
        return false;
    }

    auto res = CFile::WriteString(WTL("Brian Batista"), WTL("text.txt"));

    return res;
}

void CEngine::Shutdown()
{
    CFileSystem::Shutdown();
}

void CEngine::Tick()
{
}

bool CEngine::ProcessArguments(TArray<tchar *, TAllocator<i32>> &&)
{
    return true;
}

bool EngineInitialize(TArray<tchar *, TAllocator<i32>> &&arguments)
{
    return CEngine::Get().Initialize(std::move(arguments));
}

void EngineShutdown()
{
}

void EngineLoop()
{
}
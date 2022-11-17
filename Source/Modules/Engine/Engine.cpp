#include "Engine.hpp"
#include "HAL/FileSystem.hpp"

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

    return true;
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
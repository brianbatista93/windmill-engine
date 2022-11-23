#include "Engine.hpp"
#include "Containers/ArrayView.hpp"
#include "HAL/FileSystem.hpp"

#include "HAL/File.hpp"

CEngine &CEngine::Get()
{
    static CEngine sInstance;
    return sInstance;
}

bool CEngine::Initialize(const TArrayView<tchar *> &arguments)
{
    if (!ProcessArguments(arguments))
    {
        return false;
    }

    if (!CFileSystem::Initialize())
    {
        return false;
    }

    return false;
}

void CEngine::Shutdown()
{
    CFileSystem::Shutdown();
}

void CEngine::Tick()
{
}

bool CEngine::ProcessArguments(const TArrayView<tchar *> &)
{
    return true;
}

bool EngineInitialize(const TArrayView<tchar *> &arguments)
{
    return CEngine::Get().Initialize(arguments);
}

void EngineShutdown()
{
}

void EngineLoop()
{
}
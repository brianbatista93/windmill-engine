#include "FileSystem.hpp"
#include "Interfaces/IFileSystem.hpp"

bool CFileSystem::Initialize()
{
    return Get()->Initialize();
}

void CFileSystem::Shutdown()
{
    return Get()->Shutdown();
}

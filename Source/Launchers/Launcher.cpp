#include "Engine.hpp"
#include "Globals.hpp"

i32 WindmillMain(i32 nArgC, tchar *ppArgV[])
{
    TArray<tchar *, TAllocator<i32>> arguments(nArgC);
    for (i32 i = 0; i < nArgC; ++i)
    {
        arguments[i] = ppArgV[i];
    }

    if (!EngineInitialize(std::move(arguments)))
    {
        return -1;
    }

    while (!gIsExiting)
    {
        EngineLoop();
    }

    EngineShutdown();

    return 0;
}

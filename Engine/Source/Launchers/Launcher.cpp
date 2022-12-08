#include <iostream>
#include <locale>

#include "Containers/Array.hpp"
#include "Containers/ArrayView.hpp"
#include "Containers/ContainerFwd.hpp"
#include "Engine.hpp"
#include "Globals.hpp"

i32 WindmillMain(i32 nArgC, tchar *ppArgV[])
{
    setlocale(LC_ALL, "en_US.UTF-8");

    TArray<tchar *> arguments(nArgC);
    for (i32 i = 0; i < nArgC; ++i)
    {
        arguments[i] = ppArgV[i];
    }

    if (!EngineInitialize(arguments))
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
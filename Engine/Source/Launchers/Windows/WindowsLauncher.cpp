#include "WindowsInclude.hpp"

#ifdef WE_DEBUG
    #include <crtdbg.h>
#endif // WE_DEBUG

#include <stdio.h>

#include "Types.hpp"

extern "C"
{
    // https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    // https://gpuopen.com/amdpowerxpressrequesthighperformance/
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

extern i32 WindmillMain(i32 nArgC, tchar *ppArgV[]);

void InvalidParameterHandler(const tchar *pExpression, const tchar *pFunction, const tchar *pFile, u32 nLine, uintptr_t Reserved);

static LONG WINAPI DumpStackTrace(EXCEPTION_POINTERS *pExceptionInfo);

i32 WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ i32 nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nShowCmd);

    _set_invalid_parameter_handler(InvalidParameterHandler);

#ifdef WE_DEBUG
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    _CrtSetDebugFillThreshold(0);
#endif // WE_DEBUG

    i32 nArgC = 0;
    tchar **pArgV = ::CommandLineToArgvW(lpCmdLine, &nArgC);
    if (pArgV == nullptr)
    {
        return -1;
    }

    ::SetUnhandledExceptionFilter(&DumpStackTrace);

    const i32 nErrorCode = WindmillMain(nArgC, pArgV);

    LocalFree(pArgV);

    return nErrorCode;
}

void InvalidParameterHandler(const tchar *pExpression, const tchar *pFunction, const tchar *pFile, u32 nLine, uintptr_t)
{
    wprintf(TEXT("SECURE CRT: Invalid parameter detected.\nExpression: %s Function: %s. File: %s Line: %u\n"),
            pExpression ? pExpression : TEXT("Unknown"), pFunction ? pFunction : TEXT("Unknown"), pFile ? pFile : TEXT("Unknown"), nLine);
}

LONG WINAPI DumpStackTrace([[maybe_unused]] EXCEPTION_POINTERS *pExceptionInfo)
{
    return EXCEPTION_EXECUTE_HANDLER;
}

#ifdef WE_DEBUG
    #include "WindowsInclude.hpp"
    #include <dbghelp.h>
    #pragma comment(lib, "Dbghelp.lib")
#endif // WE_DEBUG

#include "OS/Utils.hpp"

namespace OS
{
#if WE_OS_SUPPORT_CALLSTACK_INFO
u32 GetStackTrace([[maybe_unused]] u32 nFramesToSkip, [[maybe_unused]] u32 nFramesToCapture, [[maybe_unused]] ansi ppSymbols[][MAX_SIMBOLS_LENGTH],
                  [[maybe_unused]] u32 nSymbolMaxLength, [[maybe_unused]] void *ppCallers[])
{
    typedef USHORT(WINAPI * CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID *, __out_opt PULONG);
    static auto func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary(TEXT("kernel32.dll")), "RtlCaptureStackBackTrace"));

    if (func != nullptr)
    {
        HANDLE process = ::GetCurrentProcess();
        SymInitialize(process, nullptr, TRUE);
        const u32 frames = (func)(nFramesToSkip, nFramesToCapture, ppCallers, nullptr);
        auto *symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + MAX_SIMBOLS_LENGTH * sizeof(char), 1);
        if (symbol != nullptr)
        {
            symbol->MaxNameLen = nSymbolMaxLength;
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

            for (u32 i = 0; i < frames; ++i)
            {
                SymFromAddr(process, (DWORD64)ppCallers[i], nullptr, symbol);
                strcpy_s(ppSymbols[i], symbol->Name);
            }

            free(symbol);
        }
        return frames;
    }

    return 0;
}
#endif // WE_OS_SUPPORT_CALLSTACK_INFO
} // namespace OS

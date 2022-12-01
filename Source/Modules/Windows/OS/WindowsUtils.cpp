#ifdef WE_DEBUG
    #include "WindowsInclude.hpp"
    #include <dbghelp.h>
    #pragma comment(lib, "Dbghelp.lib")
#endif // WE_DEBUG

#include "OS/Utils.hpp"

namespace OS
{
#if WE_OS_SUPPORT_CALLSTACK_INFO
u32 GetStackTrace([[maybe_unused]] u32 nFramesToSkip, [[maybe_unused]] u32 nFramesToCapture, [[maybe_unused]] ansi ppSymbols[][255],
                  [[maybe_unused]] u32 nSymbolMaxLength, [[maybe_unused]] void *ppCallers[])
{
    typedef USHORT(WINAPI * CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID *, __out_opt PULONG);
    static CaptureStackBackTraceType func =
        (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary(TEXT("kernel32.dll")), "RtlCaptureStackBackTrace"));

    if (func)
    {
        HANDLE process = ::GetCurrentProcess();
        SymInitialize(process, NULL, TRUE);
        u32 frames = (func)(nFramesToSkip, nFramesToCapture, ppCallers, NULL);
        SYMBOL_INFO *symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        if (symbol)
        {
            symbol->MaxNameLen = nSymbolMaxLength;
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

            for (u32 i = 0; i < frames; ++i)
            {
                SymFromAddr(process, (DWORD64)ppCallers[i], 0, symbol);
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
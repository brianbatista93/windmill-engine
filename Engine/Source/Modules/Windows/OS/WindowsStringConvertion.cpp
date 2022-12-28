#include "OS/StringConvertion.hpp"
#include "WindowsInclude.hpp"

namespace OS
{
usize TCharToUTF8(utf8 *pDst, const tchar *pSrc, usize nSrcLength)
{
    const i32 bufferSize = WideCharToMultiByte(CP_UTF8, 0, pSrc, (i32)nSrcLength, nullptr, 0, nullptr, nullptr);
    if (pDst == nullptr)
    {
        return usize(bufferSize);
    }
    return (usize)WideCharToMultiByte(CP_UTF8, 0, pSrc, (i32)nSrcLength, (LPSTR)pDst, bufferSize, nullptr, nullptr);
}

usize UTF8ToTChar(tchar *pDst, const utf8 *pSrc, usize nSrcLength)
{
    const i32 bufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)pSrc, (i32)nSrcLength, nullptr, 0);
    if (pDst == nullptr)
    {
        return usize(bufferSize);
    }
    return (usize)MultiByteToWideChar(CP_UTF8, 0, (LPCCH)pSrc, (i32)nSrcLength, pDst, bufferSize << 1);
}
} // namespace OS

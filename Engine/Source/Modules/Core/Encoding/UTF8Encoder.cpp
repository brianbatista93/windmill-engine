#include "UTF8Encoder.hpp"
#include "OS/StringConvertion.hpp"

constexpr bool IsValid(u32 codePoint)
{
    return (codePoint > 0x10FFFF) or (codePoint == 0xFFFE) or (codePoint == 0xFFFF);
}

usize CUTF8Encoder::GetLength(const u8 *pBytes, usize nByteCount)
{
    return OS::TCharToUTF8(nullptr, (const tchar *)pBytes, nByteCount);
}

usize CUTF8Encoder::Decode(tchar *pDest, const u8 *pSrc, usize nByteCount)
{
    return OS::UTF8ToTChar(pDest, (const utf8 *)pSrc, nByteCount);
}

usize CUTF8Encoder::Encode(utf8 *pDest, const tchar *pSrc, usize nByteCount)
{
    const usize newByteCount = nByteCount / sizeof(tchar);
    if (pDest == nullptr)
    {
        return GetLength((const u8 *)pSrc, newByteCount);
    }
    return OS::TCharToUTF8(pDest, pSrc, newByteCount);
}

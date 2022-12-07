#include "UTF8Encoder.hpp"
#include "OS/StringConvertion.hpp"

CUTF8Encoder &CUTF8Encoder::Get()
{
    static CUTF8Encoder sInstance;
    return sInstance;
}

constexpr bool IsValid(u32 codePoint)
{
    if ((codePoint > 0x10FFFF) or (codePoint == 0xFFFE) or (codePoint == 0xFFFF))
    {
        return false;
    }
    return true;
}

usize CUTF8Encoder::GetLength(const u8 *pBytes, usize nSize) const
{
    return OS::TCharToUTF8(nullptr, (const tchar *)pBytes, nSize);
}

usize CUTF8Encoder::Decode(tchar *pDest, const u8 *pSrc, usize nByteCount) const
{
    return OS::UTF8ToTChar(pDest, (const utf8 *)pSrc, nByteCount);
}

usize CUTF8Encoder::Encode(utf8 *pDest, const tchar *pSrc, usize nByteCount) const
{
    const usize newByteCount = nByteCount / sizeof(tchar);
    if (pDest == nullptr)
    {
        return GetLength((const u8 *)pSrc, newByteCount);
    }
    return OS::TCharToUTF8(pDest, pSrc, newByteCount);
}
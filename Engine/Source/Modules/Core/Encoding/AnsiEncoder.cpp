#include "AnsiEncoder.hpp"

usize CAnsiEncoder::GetLength([[maybe_unused]] const u8 *pBytes, usize nByteCount)
{
    we_assert(pBytes != nullptr and nByteCount > 0);
    return nByteCount;
}

usize CAnsiEncoder::Decode(tchar *pDest, const u8 *pSrc, usize nByteCount)
{
    we_assert(pDest != nullptr and pSrc != nullptr and nByteCount > 0);

    usize length = 0;

    while (nByteCount != 0)
    {
        *pDest = tchar(*pSrc);

        ++length;
        ++pSrc;
        ++pDest;
        --nByteCount;
    }

    return length;
}

usize CAnsiEncoder::Encode(ansi *pDest, const tchar *pSrc, usize nByteCount)
{
    we_assert(pSrc and *pSrc);

    if (pDest == nullptr)
    {
        return nByteCount / sizeof(tchar);
    }

    usize written = 0;

    for (usize i = 0; i < nByteCount; i += sizeof(tchar), ++written)
    {
        *pDest++ = (ansi)*pSrc++;
    }

    return written;
}

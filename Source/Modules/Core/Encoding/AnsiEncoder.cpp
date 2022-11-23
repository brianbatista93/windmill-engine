#include "AnsiEncoder.hpp"

CAnsiEncoder &CAnsiEncoder::Get()
{
    static CAnsiEncoder sInstance;
    return sInstance;
}

usize CAnsiEncoder::GetLength(const u8 *pBytes, usize nByteCount) const
{
    we_assert(pBytes != nullptr and nByteCount > 0);

    return nByteCount;
}

usize CAnsiEncoder::Decode(tchar *pDest, const u8 *pSrc, usize nByteCount) const
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

usize CAnsiEncoder::Encode(ansi *pDest, const tchar *pSrc, usize nByteCount) const
{
    we_assert(pSrc and *pSrc);

    if (pDest == nullptr)
    {
        return nByteCount >> 1;
    }

    usize written = 0;

    for (usize i = 0; i < nByteCount; i += 2, ++written)
    {
        *pDest++ = (ansi)*pSrc++;
    }

    return written;
}

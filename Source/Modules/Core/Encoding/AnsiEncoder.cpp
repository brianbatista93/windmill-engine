#include "AnsiEncoder.hpp"

AnsiEncoder &AnsiEncoder::Get()
{
    static AnsiEncoder sInstance;
    return sInstance;
}

usize AnsiEncoder::GetLength([[maybe_unused]] const u8 *pBytes, usize nByteCount) const
{
    we_assert(pBytes != nullptr and nByteCount > 0);

    return nByteCount;
}

usize AnsiEncoder::Decode(tchar *pDest, const u8 *pSrc, usize nByteCount) const
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
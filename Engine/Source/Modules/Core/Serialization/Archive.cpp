#include <memory>

#include "Archive.hpp"

CArchive::CArchive() : mPackageVersion(1), mEngineVersion(1), mNetworkVersion(1)
{
}

void CArchive::ClearErrors()
{
    mHasErrors = false;
}

u8 *SwapBytes(void *pValue, usize nSize)
{
    u8 *pBytes = (u8 *)pValue;
    usize top = nSize - 1;
    usize bottom = 0;
    while (bottom < top)
    {
        std::swap(pBytes[top--], pBytes[bottom++]);
    }

    return pBytes;
}

void SerializeSwapped(CArchive &archive, void *pValue, usize nSize)
{
    if (archive.IsReading())
    {
        archive.Serialize(pValue, nSize);
        pValue = SwapBytes(pValue, nSize);
    }
    else
    {
        u8 *swappedValue = SwapBytes(pValue, nSize);
        archive.Serialize(&swappedValue, nSize);
    }
}

void CArchive::EndianSerialize(void *pData, usize nSize)
{
    if /* UNLIKELY */ (IsByteSwapping())
    {
        SerializeSwapped(*this, pData, nSize);
    }
    else
    {
        Serialize(pData, nSize);
    }
}

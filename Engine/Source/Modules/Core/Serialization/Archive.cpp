#include "Archive.hpp"

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

void SerializeSwapped(CArchive &archive, void *pValue, usize nSize, const tchar *pName)
{
    if (archive.IsReading())
    {
        archive.Serialize(pValue, nSize, pName);
        pValue = SwapBytes(pValue, nSize);
    }
    else
    {
        u8 *swappedValue = SwapBytes(pValue, nSize);
        archive.Serialize(&swappedValue, nSize, pName);
    }
}

void CArchive::EndianSerialize(void *pData, usize nSize, const tchar *pName)
{
    if /* UNLIKELY */ (NeedSwapping())
    {
        SerializeSwapped(*this, pData, nSize, pName);
    }
    else
    {
        Serialize(pData, nSize, pName);
    }
}

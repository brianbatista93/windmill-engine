#include "BinaryArchive.hpp"
#include "Containers/WeString.hpp"

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

void SerializeSwapped(IBinaryArchive &archive, void *pValue, usize nSize)
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

void IBinaryArchive::EndianSerialize(void *pData, usize nSize)
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

IBinaryArchive &operator<<(IBinaryArchive &archive, CString &value)
{
    if (archive.IsReading())
    {
        i32 length = 0;
        archive << length;
        if (length)
        {
            auto& arry = value.GetArray();
            arry.Resize(length + 1);

            archive.Serialize(arry.GetData(), length * sizeof(tchar));
        }
    }
    else // Is Writting
    {
        i32 length = value.GetLength();
        archive.Serialize(&length, sizeof(length));

        if (length)
        {
            archive.Serialize((void *)*value, length * sizeof(tchar));
        }
    }

    return archive;
}

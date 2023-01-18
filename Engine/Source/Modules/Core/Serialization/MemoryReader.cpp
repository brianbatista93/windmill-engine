#include "MemoryReader.hpp"

void CMemoryReader::Serialize(void *pData, usize nSize)
{
    we_assert(pData && nSize);
    we_assert(mOffset + nSize <= mBuffer.GetSize());

    std::memcpy(pData, mBuffer.GetData() + mOffset, nSize);

    mOffset += nSize;
}

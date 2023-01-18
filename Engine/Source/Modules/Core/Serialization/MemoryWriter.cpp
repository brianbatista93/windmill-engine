#include "MemoryWriter.hpp"

void CMemoryWriter::Serialize(void *pData, usize nSize)
{
    we_assert(pData && nSize <= (mBuffer.GetCapacity() - mOffset));

    std::memcpy(mBuffer.GetData() + mOffset, pData, nSize);

    mOffset += nSize;
}

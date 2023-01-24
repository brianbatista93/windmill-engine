#include "MemoryWriter.hpp"

void CMemoryWriter::Serialize(void *pData, usize nSize, const tchar *)
{
    we_assert(pData && nSize <= (mBuffer.GetCapacity() - mOffset));

    std::memcpy(mBuffer.GetData() + mOffset, pData, nSize);

    mOffset += nSize;
}

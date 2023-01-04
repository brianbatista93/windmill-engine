#include <bit>

#include "CRC32.hpp"

u32 CRC32::Calculate(const void *pBuffer, usize nBufferSize) noexcept
{
    const u32 crc = 0;
    return Update(crc, pBuffer, nBufferSize);
}

u32 CRC32::Update(u32 lastCrc, const void *pBuffer, usize nBufferSize) noexcept
{
    u32 crc = ~lastCrc;
    const u8 *byte = (const u8 *)pBuffer;

    while (nBufferSize--)
    {
        crc = (crc >> 8) ^ WE::Internal::kTable[(crc & 0xFF) ^ *byte++];
    }

    return ~crc;
}

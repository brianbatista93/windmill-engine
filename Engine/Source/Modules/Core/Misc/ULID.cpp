#include "ULID.hpp"
#include "HAL/Timestamp.hpp"
#include "Math/Random.hpp"

void CULID::Generate(u64 nTimestamp)
{
    we_assert(nTimestamp < (1ULL << 48));

    EncodeTimestamp(nTimestamp);
    EncodeRandom();
}

void CULID::Generate()
{
    const u64 timestamp = CTimestamp::GetTicks();
    Generate(timestamp);
}

bool CULID::IsValid() const
{
    return mData != 0;
}

void CULID::EncodeTimestamp(u64 nTimestamp)
{
    mData[0] = static_cast<uint8_t>(nTimestamp >> 40);
    mData[1] = static_cast<uint8_t>(nTimestamp >> 32);
    mData[2] = static_cast<uint8_t>(nTimestamp >> 24);
    mData[3] = static_cast<uint8_t>(nTimestamp >> 16);
    mData[4] = static_cast<uint8_t>(nTimestamp >> 8);
    mData[5] = static_cast<uint8_t>(nTimestamp);
}

void CULID::EncodeRandom()
{
    constexpr auto randFunc = []() {
        return (u8)CRandom::GetRanged<u32>(0, 255);
    };

    mData[6] = randFunc();
    mData[7] = randFunc();
    mData[8] = randFunc();
    mData[9] = randFunc();
    mData[10] = randFunc();
    mData[11] = randFunc();
    mData[12] = randFunc();
    mData[13] = randFunc();
    mData[14] = randFunc();
    mData[15] = randFunc();
}

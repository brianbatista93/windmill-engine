#include "ULID.hpp"
#include "HAL/Timestamp.hpp"
#include "Math/Random.hpp"

// NOLINTBEGIN

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

static const char Encoding[33] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

inline void ToMarshal(const CULID &ulid, tchar dst[26])
{
    // 10 byte timestamp
    dst[0] = Encoding[(ulid.GetData()[0] & 224) >> 5];
    dst[1] = Encoding[ulid.GetData()[0] & 31];
    dst[2] = Encoding[(ulid.GetData()[1] & 248) >> 3];
    dst[3] = Encoding[((ulid.GetData()[1] & 7) << 2) | ((ulid.GetData()[2] & 192) >> 6)];
    dst[4] = Encoding[(ulid.GetData()[2] & 62) >> 1];
    dst[5] = Encoding[((ulid.GetData()[2] & 1) << 4) | ((ulid.GetData()[3] & 240) >> 4)];
    dst[6] = Encoding[((ulid.GetData()[3] & 15) << 1) | ((ulid.GetData()[4] & 128) >> 7)];
    dst[7] = Encoding[(ulid.GetData()[4] & 124) >> 2];
    dst[8] = Encoding[((ulid.GetData()[4] & 3) << 3) | ((ulid.GetData()[5] & 224) >> 5)];
    dst[9] = Encoding[ulid.GetData()[5] & 31];

    // 16 bytes of entropy
    dst[10] = Encoding[(ulid.GetData()[6] & 248) >> 3];
    dst[11] = Encoding[((ulid.GetData()[6] & 7) << 2) | ((ulid.GetData()[7] & 192) >> 6)];
    dst[12] = Encoding[(ulid.GetData()[7] & 62) >> 1];
    dst[13] = Encoding[((ulid.GetData()[7] & 1) << 4) | ((ulid.GetData()[8] & 240) >> 4)];
    dst[14] = Encoding[((ulid.GetData()[8] & 15) << 1) | ((ulid.GetData()[9] & 128) >> 7)];
    dst[15] = Encoding[(ulid.GetData()[9] & 124) >> 2];
    dst[16] = Encoding[((ulid.GetData()[9] & 3) << 3) | ((ulid.GetData()[10] & 224) >> 5)];
    dst[17] = Encoding[ulid.GetData()[10] & 31];
    dst[18] = Encoding[(ulid.GetData()[11] & 248) >> 3];
    dst[19] = Encoding[((ulid.GetData()[11] & 7) << 2) | ((ulid.GetData()[12] & 192) >> 6)];
    dst[20] = Encoding[(ulid.GetData()[12] & 62) >> 1];
    dst[21] = Encoding[((ulid.GetData()[12] & 1) << 4) | ((ulid.GetData()[13] & 240) >> 4)];
    dst[22] = Encoding[((ulid.GetData()[13] & 15) << 1) | ((ulid.GetData()[14] & 128) >> 7)];
    dst[23] = Encoding[(ulid.GetData()[14] & 124) >> 2];
    dst[24] = Encoding[((ulid.GetData()[14] & 3) << 3) | ((ulid.GetData()[15] & 224) >> 5)];
    dst[25] = Encoding[ulid.GetData()[15] & 31];
}

CString CULID::ToString() const
{
    tchar marshal[27]{0};
    ToMarshal(*this, marshal);
    return {marshal, 26};
}

u64 CULID::GetTimestamp() const
{
    u64 ticks = 0;

    ticks |= mData[0];

    ticks <<= 8;
    ticks |= mData[1];

    ticks <<= 8;
    ticks |= mData[2];

    ticks <<= 8;
    ticks |= mData[3];

    ticks <<= 8;
    ticks |= mData[4];

    ticks <<= 8;
    ticks |= mData[5];

    return ticks;
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

// clang-format off
static const u8 kMarshalDecTable[256] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	/* 0     1     2     3     4     5     6     7  */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	/* 8     9                                      */
	0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	/*    10(A) 11(B) 12(C) 13(D) 14(E) 15(F) 16(G) */
	0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
	/*17(H)     18(J) 19(K)       20(M) 21(N)       */
	0x11, 0xFF, 0x12, 0x13, 0xFF, 0x14, 0x15, 0xFF,
	/*22(P)23(Q)24(R) 25(S) 26(T)       27(V) 28(W) */
	0x16, 0x17, 0x18, 0x19, 0x1A, 0xFF, 0x1B, 0x1C,
	/*29(X)30(Y)31(Z)                               */
	0x1D, 0x1E, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
// clang-format on

bool CULID::Unmarshal(const tchar *pStr, CULID *pOutResult)
{
    we_assert(pStr && *pStr && pOutResult);

    const usize len = std::char_traits<tchar>::length(pStr);

    if (len != 26)
    {
        return false;
    }

    pOutResult->mData[0] = (kMarshalDecTable[int(pStr[0])] << 5) | kMarshalDecTable[int(pStr[1])];
    pOutResult->mData[1] = (kMarshalDecTable[int(pStr[2])] << 3) | (kMarshalDecTable[int(pStr[3])] >> 2);
    pOutResult->mData[2] = (kMarshalDecTable[int(pStr[3])] << 6) | (kMarshalDecTable[int(pStr[4])] << 1) | (kMarshalDecTable[int(pStr[5])] >> 4);
    pOutResult->mData[3] = (kMarshalDecTable[int(pStr[5])] << 4) | (kMarshalDecTable[int(pStr[6])] >> 1);
    pOutResult->mData[4] = (kMarshalDecTable[int(pStr[6])] << 7) | (kMarshalDecTable[int(pStr[7])] << 2) | (kMarshalDecTable[int(pStr[8])] >> 3);
    pOutResult->mData[5] = (kMarshalDecTable[int(pStr[8])] << 5) | kMarshalDecTable[int(pStr[9])];

    // entropy
    pOutResult->mData[6] = (kMarshalDecTable[int(pStr[10])] << 3) | (kMarshalDecTable[int(pStr[11])] >> 2);
    pOutResult->mData[7] = (kMarshalDecTable[int(pStr[11])] << 6) | (kMarshalDecTable[int(pStr[12])] << 1) | (kMarshalDecTable[int(pStr[13])] >> 4);
    pOutResult->mData[8] = (kMarshalDecTable[int(pStr[13])] << 4) | (kMarshalDecTable[int(pStr[14])] >> 1);
    pOutResult->mData[9] = (kMarshalDecTable[int(pStr[14])] << 7) | (kMarshalDecTable[int(pStr[15])] << 2) | (kMarshalDecTable[int(pStr[16])] >> 3);
    pOutResult->mData[10] = (kMarshalDecTable[int(pStr[16])] << 5) | kMarshalDecTable[int(pStr[17])];
    pOutResult->mData[11] = (kMarshalDecTable[int(pStr[18])] << 3) | (kMarshalDecTable[int(pStr[19])] >> 2);
    pOutResult->mData[12] = (kMarshalDecTable[int(pStr[19])] << 6) | (kMarshalDecTable[int(pStr[20])] << 1) | (kMarshalDecTable[int(pStr[21])] >> 4);
    pOutResult->mData[13] = (kMarshalDecTable[int(pStr[21])] << 4) | (kMarshalDecTable[int(pStr[22])] >> 1);
    pOutResult->mData[14] = (kMarshalDecTable[int(pStr[22])] << 7) | (kMarshalDecTable[int(pStr[23])] << 2) | (kMarshalDecTable[int(pStr[24])] >> 3);
    pOutResult->mData[15] = (kMarshalDecTable[int(pStr[24])] << 5) | kMarshalDecTable[int(pStr[25])];

    return true;
}

i32 CULID::Compare(const CULID &lhs, const CULID &rhs)
{
    for (int i = 0; i < 16; i++)
    {
        if (lhs.mData[i] != rhs.mData[i])
        {
            return (lhs.mData[i] < rhs.mData[i]) * -2 + 1;
        }
    }

	return 0;
}

// NOLINTEND

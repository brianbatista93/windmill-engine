#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Serialization/Archive.hpp"

class CArchiveMock : public CArchive
{
  public:
    MOCK_METHOD(void, Serialize, (void *pData, usize nSize), (final));
};

TEST(Serialization, BasicTypes)
{
    CArchiveMock archive;

    u8 u8Value = 0;
    u16 u16Value = 0;
    u32 u32Value = 0;
    u64 u64Value = 0;
    i8 i8Value = 0;
    i16 i16Value = 0;
    i32 i32Value = 0;
    i64 i64Value = 0;
    f32 f32Value = 0.0f;
    f64 f64Value = 0.0;
    ansi ansiValue = 0;
    wide wideValue = 0;
    bool boolValue = false;

    EXPECT_CALL(archive, Serialize).Times(13);

    archive << u8Value;
    archive << u16Value;
    archive << u32Value;
    archive << u64Value;
    archive << i8Value;
    archive << i16Value;
    archive << i32Value;
    archive << i64Value;
    archive << f32Value;
    archive << f64Value;
    archive << ansiValue;
    archive << wideValue;
    archive << boolValue;
}

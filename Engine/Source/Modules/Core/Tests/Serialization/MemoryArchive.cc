#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Containers/WeString.hpp"
#include "Serialization/MemoryWriter.hpp"

TEST(Serialization, BasicTypes)
{
    CMemoryWriter archive{128};

    u8 u8Value = 0;
    u16 u16Value = 1;
    u32 u32Value = 2;
    u64 u64Value = 3;
    i8 i8Value = 4;
    i16 i16Value = 5;
    i32 i32Value = 6;
    i64 i64Value = 7;
    f32 f32Value = 8.0f;
    f64 f64Value = 9.0;
    ansi ansiValue = 10;
    wide wideValue = 11;
    bool boolValue = false;

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

    CString text = WTL("The brown fox jumps over the lazy dog.");
    archive << text;
}

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Containers/WeString.hpp"
#include "Serialization/MemoryReader.hpp"
#include "Serialization/MemoryWriter.hpp"

TEST(Serialization, BasicTypes)
{
    CArray<u8> buffer{128};
    {
        CMemoryWriter writer{128};

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
        bool boolValue = true;

        writer << u8Value;
        writer << u16Value;
        writer << u32Value;
        writer << u64Value;
        writer << i8Value;
        writer << i16Value;
        writer << i32Value;
        writer << i64Value;
        writer << f32Value;
        writer << f64Value;
        writer << ansiValue;
        writer << wideValue;
        writer << boolValue;

        CString text = WTL("The brown fox jumps over the lazy dog.");
        writer << text;

        buffer = writer.ToArray();
    }

    {
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
        CString text;

        CMemoryReader reader(buffer);

        reader << u8Value;
        reader << u16Value;
        reader << u32Value;
        reader << u64Value;
        reader << i8Value;
        reader << i16Value;
        reader << i32Value;
        reader << i64Value;
        reader << f32Value;
        reader << f64Value;
        reader << ansiValue;
        reader << wideValue;
        reader << boolValue;
        reader << text;

        EXPECT_EQ(u8Value, 0);
        EXPECT_EQ(u16Value, 1);
        EXPECT_EQ(u32Value, 2);
        EXPECT_EQ(u64Value, 3);
        EXPECT_EQ(i8Value, 4);
        EXPECT_EQ(i16Value, 5);
        EXPECT_EQ(i32Value, 6);
        EXPECT_EQ(i64Value, 7);
        EXPECT_EQ(f32Value, 8);
        EXPECT_EQ(f64Value, 9);
        EXPECT_EQ(ansiValue, 10);
        EXPECT_EQ(wideValue, 11);
        EXPECT_EQ(boolValue, true);
        EXPECT_EQ(text.GetLength(), 38);
    }
}

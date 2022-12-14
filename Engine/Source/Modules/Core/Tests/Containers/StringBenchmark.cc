#include <gtest/gtest.h>

#include "Containers/WeString.hpp"
#include "Encoding/AnsiEncoder.hpp"
#include "Types.hpp"

TEST(String, EmptyInit)
{
    CString empty;
    EXPECT_TRUE(empty.IsEmpty()) << "Empty string should be empty";
    EXPECT_EQ(empty.GetLength(), 0) << "Empty string should have length 0";
}

TEST(String, Literals)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    EXPECT_FALSE(lit.IsEmpty()) << "Literal string should not be empty";
    EXPECT_EQ(lit.GetLength(), 44) << "Literal string should have length 44";
}

TEST(String, AppendChar)
{
    CString lit = WTL("The quick brown fox jumps over the lazy do");
    lit = lit + WT('g');
    lit += WT('.');

    EXPECT_FALSE(lit.IsEmpty());
    EXPECT_EQ(lit.GetLength(), 44);
}

TEST(String, AppendCString)
{
    CString lit = WTL("The quick brown fox");
    lit = lit + WT(" jumps over");
    lit += WT(" the lazy dog.");

    EXPECT_FALSE(lit.IsEmpty());
    EXPECT_EQ(lit.GetLength(), 44);
}

TEST(String, StartsWith)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    EXPECT_TRUE(lit.StartsWith(WT("The")));
    EXPECT_FALSE(lit.StartsWith(WT("the")));
}

TEST(String, EndsWith)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    EXPECT_TRUE(lit.EndsWith(WT("dog.")));
    EXPECT_FALSE(lit.EndsWith(WT("Dog.")));
}

TEST(String, Substring)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    CString sub = lit.Substring(10, 5);
    EXPECT_EQ(sub, WTL("brown"));
}

TEST(String, Find)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    EXPECT_EQ(lit.Find(WT("brown")), 10);
    EXPECT_EQ(lit.Find(WT("dog")), 40);
    EXPECT_EQ(lit.Find(WT("fox")), 16);
    EXPECT_EQ(lit.Find(WT("fox"), 17), -1);
}

TEST(String, Split)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    auto parts = lit.Split(WT(" "));
    EXPECT_EQ(parts.GetSize(), 9);
    EXPECT_EQ(parts[0], WTL("The"));
    EXPECT_EQ(parts[1], WTL("quick"));
    EXPECT_EQ(parts[2], WTL("brown"));
    EXPECT_EQ(parts[3], WTL("fox"));
    EXPECT_EQ(parts[4], WTL("jumps"));
    EXPECT_EQ(parts[5], WTL("over"));
    EXPECT_EQ(parts[6], WTL("the"));
    EXPECT_EQ(parts[7], WTL("lazy"));
    EXPECT_EQ(parts[8], WTL("dog."));
}

TEST(String, Join)
{
    auto cart = {WTL("Orange"), WTL("Apple"), WTL("Pear"), WTL("Melon"), WTL("Grape")};

    CString joined = CString::Join(WT(";\n"), cart);
    EXPECT_EQ(joined, WTL("Orange;\nApple;\nPear;\nMelon;\nGrape"));
}

TEST(String, CAnsiEncoder)
{
    u8 bytes[] = {0x54, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b, 0x20, 0x62, 0x72, 0x6f, 0x77, 0x6e, 0x20, 0x66, 0x6f, 0x78, 0x20, 0x6a, 0x75,
                  0x6d, 0x70, 0x73, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x61, 0x7a, 0x79, 0x20, 0x64, 0x6f, 0x67, 0x2e};

    CString text(bytes, 44, CAnsiEncoder{});
    EXPECT_EQ(text, WTL("The quick brown fox jumps over the lazy dog."));
}

TEST(String, Format)
{
    CString text = CString::Format(WT("This is {0}."), WT("Windmill Engine"));
    EXPECT_EQ(text, WTL("This is Windmill Engine."));
}

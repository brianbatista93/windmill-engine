#include <gtest/gtest.h>

#include "HAL/Path.hpp"

TEST(Path, EmptyInit)
{
    CPath path;
    EXPECT_TRUE(path.IsEmpty());
}

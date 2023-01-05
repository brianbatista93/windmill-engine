#include <gtest/gtest.h>

#include "Misc/ULID.hpp"

TEST(ULID, Generate)
{
    CULID ulid;
    ulid.Generate();
    EXPECT_TRUE(ulid.IsValid());
}

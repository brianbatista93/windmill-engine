#include <gtest/gtest.h>

#include "Misc/ULID.hpp"

TEST(ULID, Generate)
{
    CULID ulid;
    ulid.Generate();
    EXPECT_TRUE(ulid.IsValid());
}

TEST(ULID, Parse)
{
    CULID ulid;
    const bool result = TryParse(WT("0001C7STHC0G2081040G208104"), &ulid);
    EXPECT_TRUE(result);
}

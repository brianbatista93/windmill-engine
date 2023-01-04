#include <gtest/gtest.h>

#include "HAL/Timestamp.hpp"

TEST(Time, DefaultCall)
{
    EXPECT_GT(CTimestamp::GetTicks(), 0);
    EXPECT_GT(CTimestamp::GetSeconds(), 0.0);
    EXPECT_GT(CTimestamp::GetMilliseconds(), 0.0);
    EXPECT_GT(CTimestamp::GetMicroseconds(), 0.0);
    EXPECT_GT(CTimestamp::GetNanoseconds(), 0.0);
}

TEST(Time, SystemTime)
{
    i32 year = 0;
    i32 month = 0;
    i32 dayOfWeek = 0;
    i32 day = 0;
    i32 hour = 0;
    i32 min = 0;
    i32 sec = 0;
    i32 msec = 0;
    CTimestamp::GetSystemTime(year, month, dayOfWeek, day, hour, min, sec, msec);

    EXPECT_GE(year, 2023);
    EXPECT_GE(day, 1);
}

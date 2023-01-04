/*
Copyright (C) 2021-2022 Bull Technology, Ltd.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

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

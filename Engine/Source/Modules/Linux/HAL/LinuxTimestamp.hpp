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

#include "OS/ITimestamp.hpp"

class CLinuxTimestamp : public ITimestamp
{
  public:
    static u64 GetTicks();
    static f64 GetSeconds();
    static f64 GetMilliseconds();
    static f64 GetMicroseconds();
    static f64 GetNanoseconds();

    static void GetSystemTime(i32 &year, i32 &month, i32 &dayOfWeek, i32 &day, i32 &hour, i32 &min, i32 &sec, i32 &msec);
    static void GetUTCTime(i32 &year, i32 &month, i32 &dayOfWeek, i32 &day, i32 &hour, i32 &min, i32 &sec, i32 &msec);
};

using CTimestamp = CLinuxTimestamp;

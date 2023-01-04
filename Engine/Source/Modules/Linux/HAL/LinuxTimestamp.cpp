#include <ctime>
#include <sys/time.h>

#include "LinuxTimestamp.hpp"

// NOLINTBEGIN(bugprone-narrowing-conversions)

u64 CLinuxTimestamp::GetTicks()
{
    struct timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);
    return static_cast<u64>(timeSpec.tv_sec) * 1e9 + static_cast<u64>(timeSpec.tv_nsec);
}

f64 CLinuxTimestamp::GetSeconds()
{
    struct timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);
    return static_cast<f64>(timeSpec.tv_sec) + static_cast<f64>(timeSpec.tv_nsec) / 1e9;
}

f64 CLinuxTimestamp::GetMilliseconds()
{
    return GetSeconds() * 1000.0;
}

f64 CLinuxTimestamp::GetMicroseconds()
{
    return GetMilliseconds() * 1000.0;
}

f64 CLinuxTimestamp::GetNanoseconds()
{
    return GetMicroseconds() * 1000.0;
}

void CLinuxTimestamp::GetSystemTime(i32 &year, i32 &month, i32 &dayOfWeek, i32 &day, i32 &hour, i32 &min, i32 &sec, i32 &msec)
{
    struct timeval time;
    gettimeofday(&time, nullptr);

    struct tm timeInfo;
    localtime_r(&time.tv_sec, &timeInfo);

    year = timeInfo.tm_year + 1900;
    month = timeInfo.tm_mon + 1;
    dayOfWeek = timeInfo.tm_wday;
    day = timeInfo.tm_mday;
    hour = timeInfo.tm_hour;
    min = timeInfo.tm_min;
    sec = timeInfo.tm_sec;
    msec = time.tv_usec / 1000;
}

void CLinuxTimestamp::GetUTCTime(i32 &year, i32 &month, i32 &dayOfWeek, i32 &day, i32 &hour, i32 &min, i32 &sec, i32 &msec)
{
    struct timeval time;
    gettimeofday(&time, nullptr);

    struct tm timeInfo;
    gmtime_r(&time.tv_sec, &timeInfo);

    year = timeInfo.tm_year + 1900;
    month = timeInfo.tm_mon + 1;
    dayOfWeek = timeInfo.tm_wday;
    day = timeInfo.tm_mday;
    hour = timeInfo.tm_hour;
    min = timeInfo.tm_min;
    sec = timeInfo.tm_sec;
    msec = time.tv_usec / 1000;
}

// NOLINTEND(bugprone-narrowing-conversions)

#include "WindowsTimestamp.hpp"
#include "WindowsInclude.hpp"

u64 CWindowsTimestamp::GetTicks()
{
    LARGE_INTEGER ticks{};
    if (::QueryPerformanceCounter(&ticks))
    {
        return ticks.QuadPart;
    }

    return 0;
}

f64 CWindowsTimestamp::GetSeconds()
{
    const u64 ticks = GetTicks();
    LARGE_INTEGER frequency;

    if (ticks && ::QueryPerformanceFrequency(&frequency))
    {
        return static_cast<f64>(ticks) / static_cast<f64>(frequency.QuadPart);
    }

    return 0.0;
}

f64 CWindowsTimestamp::GetMilliseconds()
{
    return GetSeconds() * 1000.0;
}

f64 CWindowsTimestamp::GetMicroseconds()
{
    return GetMilliseconds() * 1000.0;
}

f64 CWindowsTimestamp::GetNanoseconds()
{
    return GetMicroseconds() * 1000.0;
}

void CWindowsTimestamp::GetSystemTime(i32 &year, i32 &month, i32 &dayOfWeek, i32 &day, i32 &hour, i32 &min, i32 &sec, i32 &msec)
{
    SYSTEMTIME sysTime = {0};
    ::GetLocalTime(&sysTime);

    year = sysTime.wYear;
    month = sysTime.wMonth;
    dayOfWeek = sysTime.wDayOfWeek;
    day = sysTime.wDay;
    hour = sysTime.wHour;
    min = sysTime.wMinute;
    sec = sysTime.wSecond;
    msec = sysTime.wMilliseconds;
}

void CWindowsTimestamp::GetUTCTime(i32 &year, i32 &month, i32 &dayOfWeek, i32 &day, i32 &hour, i32 &min, i32 &sec, i32 &msec)
{
    SYSTEMTIME sysTime = {0};
    ::GetSystemTime(&sysTime);

    year = sysTime.wYear;
    month = sysTime.wMonth;
    dayOfWeek = sysTime.wDayOfWeek;
    day = sysTime.wDay;
    hour = sysTime.wHour;
    min = sysTime.wMinute;
    sec = sysTime.wSecond;
    msec = sysTime.wMilliseconds;
}

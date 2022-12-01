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

#include "Types.hpp"

enum class ELogLevel
{
    eDisabled = 0, // No logging
    eFatal,        // Fatal error
    eError,        // Error
    eWarning,      // Warning
    eInfo,         // Information
    eDebug,        // Debug
    eTrace,        // Trace
    eAll = eTrace, // All logging
    eCount         // Count of log levels
};

static constexpr const tchar *ToString(ELogLevel level)
{
    switch (level)
    {
    case ELogLevel::eDisabled:
        return WT("Disabled");
    case ELogLevel::eFatal:
        return WT("Fatal");
    case ELogLevel::eError:
        return WT("Error");
    case ELogLevel::eWarning:
        return WT("Warning");
    case ELogLevel::eInfo:
        return WT("Info");
    case ELogLevel::eDebug:
        return WT("Debug");
    case ELogLevel::eTrace:
        return WT("Trace");
    default:
        return WT("Unknown");
    }
}
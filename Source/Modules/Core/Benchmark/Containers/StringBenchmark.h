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

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include "Containers/WeString.hpp"

static void BM_StringEmptyCreation(benchmark::State &rState)
{
    for (auto _ : rState)
    {
        CString empty;
    }
}
BENCHMARK(BM_StringEmptyCreation);

static void BM_StringLiteralCreation(benchmark::State &rState)
{
    for (auto _ : rState)
    {
        CString lit = WTL("The quick brown fox jumps over the lazy dog");
        benchmark::DoNotOptimize(lit);
    }
}
BENCHMARK(BM_StringLiteralCreation);

TEST(String, EmptyInit)
{
    CString empty;
    EXPECT_TRUE(empty.IsEmpty());
    EXPECT_EQ(empty.GetLength(), 0);
}

TEST(String, Literals)
{
    CString lit = WTL("The quick brown fox jumps over the lazy dog.");
    EXPECT_FALSE(lit.IsEmpty());
    EXPECT_EQ(lit.GetLength(), 44);
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
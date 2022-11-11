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

#include "Containers/Allocator.hpp"

template <class T, typename AllocType = TAllocator<u64>> class TArray;

#include "Containers/Array.hpp"

class CObject
{
  public:
    CObject() : m_nDummy(1) {}
    CObject(int val) : m_nDummy(val) {}
    CObject(const CObject &other) : m_nDummy(other.m_nDummy) {}
    CObject(CObject &&other) noexcept : m_nDummy(other.m_nDummy) {}
    CObject &operator=(const CObject &other)
    {
        if (this != &other)
        {
            m_nDummy = other.m_nDummy;
        }

        return *this;
    }

    CObject &operator=(CObject &&other) noexcept
    {
        if (this != &other)
        {
            m_nDummy = other.m_nDummy;
            other.m_nDummy = 0;
        }
        return *this;
    }

  private:
    int m_nDummy;
};

static void BM_ArrayEmptyCreation(benchmark::State &rState)
{
    for (auto _ : rState)
    {
        TArray<CObject> emptyArray;
    }
}
BENCHMARK(BM_ArrayEmptyCreation);

static void BM_ArraySizedCreation(benchmark::State &rState)
{
    for (auto _ : rState)
    {
        TArray<CObject> sizedArray(1024);
    }
}
BENCHMARK(BM_ArraySizedCreation);

static void BM_ArrayAdd(benchmark::State &rState)
{
    TArray<i32> arr;
    for (auto _ : rState)
    {
        arr.Add(1);
    }
}
BENCHMARK(BM_ArrayAdd);

TEST(Array, EmptyInit)
{
    TArray<CObject> emptyArray;
    EXPECT_TRUE(emptyArray.IsEmpty());
    EXPECT_EQ(emptyArray.GetSize(), 0);
    EXPECT_EQ(emptyArray.GetCapacity(), 0);
}

TEST(Array, InitWithSize)
{
    TArray<CObject> array(10);
    EXPECT_FALSE(array.IsEmpty());
    EXPECT_EQ(array.GetSize(), 10);
    EXPECT_EQ(array.GetCapacity(), 10);
}

TEST(Array, InitWithList)
{
    TArray<CObject> array({CObject(1), CObject(2), CObject(3)});
    EXPECT_FALSE(array.IsEmpty());
    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.GetCapacity(), 3);
}

TEST(Array, Copy)
{
    TArray<CObject> a = {1, 2, 3};
    TArray<CObject> b = a;
    EXPECT_FALSE(b.IsEmpty());
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_EQ(b.GetCapacity(), 3);
}

TEST(Array, CopyAssaingment)
{
    TArray<CObject> a = {1, 2, 3, 4, 5};
    TArray<CObject> b = {6, 7, 8};

    b = a;

    EXPECT_FALSE(b.IsEmpty());
    EXPECT_EQ(b.GetSize(), 5);
    EXPECT_EQ(b.GetCapacity(), 5);
}

TEST(Array, MoveAssaingment)
{
    TArray<CObject> a = {1, 2, 3, 4, 5};
    TArray<CObject> b = {6, 7, 8};

    b = std::move(a);

    EXPECT_FALSE(b.IsEmpty());
    EXPECT_EQ(b.GetSize(), 5);
    EXPECT_EQ(b.GetCapacity(), 5);
}

TEST(Array, AddCopy)
{
    TArray<CObject> a = {1, 2, 3};
    a.Add(CObject(4));
    EXPECT_FALSE(a.IsEmpty());
    EXPECT_EQ(a.GetSize(), 4);
    EXPECT_EQ(a.GetCapacity(), 6);
}
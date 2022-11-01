#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

template <class T, typename AllocType = TAllocator<u64>> class TArray;

#include "Containers/Array.hpp"

class CObject
{
  public:
    CObject() : m_nDummy(1) {}
    CObject(int val) : m_nDummy(val) {}

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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest();
    if (RUN_ALL_TESTS() != 0)
    {
        return 1;
    }
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        return 1;
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
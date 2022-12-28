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

#include <algorithm>
#include <initializer_list>

#include "Allocator.hpp"
#include "Math/MathUtils.hpp"

template <class T, class Alloc>
class TArray
{
  public:
    using ElementType = T;
    using AllocatorType = Alloc;
    using IndexType = typename AllocatorType::IndexType;
    using Iterator = ElementType *;
    using ConstIterator = const ElementType *;

    inline TArray() : m_Allocator(), mSize(0), mCapacity(0) {}
    inline TArray(IndexType nInitialSize) { InitializeWithSize(nInitialSize); }
    inline TArray(std::initializer_list<ElementType> list) noexcept { InitializeWithRange(list.begin(), list.end()); }
    template <typename ItType>
    inline TArray(ItType first, ItType last)
    {
        InitializeWithRange(first, last);
    }

    inline TArray(const TArray &other) { CopyFromOther(other); }
    inline TArray(TArray &&other) noexcept { MoveFromOther(std::move(other)); }

    inline ~TArray()
    {
        CMemoryUtils::Destroy(GetData(), mSize);
        m_Allocator.ReleaseData();
    }

    inline TArray &operator=(const TArray &other) // NOLINT
    {
        if (this != &other)
        {
            const auto nDifference = CMath::Abs(mSize - other.mSize);
            CMemoryUtils::Destroy(GetData() + other.mSize, nDifference);

            const IndexType sizeInBytes = other.mSize * sizeof(ElementType);
            mAllocator.Reallocate(sizeInBytes);
            mSize = other.mSize;
            mCapacity = other.mSize;
            memcpy(GetData(), other.GetData(), sizeInBytes);
        }

        return *this;
    }

    inline TArray &operator=(TArray &&other) noexcept
    {
        if (this != &other)
        {
            m_Allocator.ReleaseData();
            other.m_Allocator.MoveTo(m_Allocator);
            mSize = other.mSize;
            mCapacity = other.mCapacity;

            other.mSize = 0;
            other.mCapacity = 0;
        }

        return *this;
    }

    inline void Reserve(IndexType nCapacity)
    {
        if (nCapacity > mCapacity)
        {
            m_Allocator.Reallocate(nCapacity * sizeof(ElementType));
            mCapacity = nCapacity;
        }
    }

    inline void Clear(bool bShrink = false)
    {
        CMemoryUtils::Destroy(GetData(), mSize);
        mSize = 0;
        if (bShrink)
        {
            ShrinkToFit();
        }
    }

    inline void ShrinkToFit()
    {
        if (mCapacity != mSize)
        {
            m_Allocator.Reallocate(mSize * sizeof(ElementType));
            mCapacity = mSize;
        }
    }

    inline IndexType RemoveAt(IndexType nIndex)
    {
        we_assert(nIndex < mSize && "Index out of bound");

        for (IndexType i = mSize; i > nIndex; --i)
        {
            GetData()[i] = std::move(GetData()[i - 1]);
        }

        return --mSize;
    }

    NDISCARD inline bool IsEmpty() const { return GetSize() == 0; }
    NDISCARD inline IndexType GetSize() const { return mSize; }
    NDISCARD inline IndexType GetCapacity() const { return mCapacity; }

    inline ElementType &At(IndexType nIndex)
    {
        we_assert(nIndex < mSize);
        return GetData()[nIndex];
    }
    NDISCARD inline const ElementType &At(IndexType nIndex) const
    {
        we_assert(nIndex < mSize);
        return GetData()[nIndex];
    }

    inline ElementType &operator[](IndexType nIndex) { return At(nIndex); }
    inline const ElementType &operator[](IndexType nIndex) const { return At(nIndex); }

    inline ElementType &Front() { return At(0); }
    NDISCARD inline const ElementType &Front() const { return At(0); }

    inline ElementType &Back() { return At(mSize - 1); }
    NDISCARD inline const ElementType &Back() const { return At(mSize - 1); }

    inline ElementType *GetData() { return (ElementType *)m_Allocator.GetData(); }
    NDISCARD inline const ElementType *GetData() const { return (const ElementType *)m_Allocator.GetData(); }

    template <class... Args>
    inline IndexType Emplace(IndexType nIndex, Args &&...vArgs) noexcept
    {
        we_assert(nIndex <= mSize && "Index out of bound");

        const IndexType newSize = mSize + 1;
        if (newSize > mCapacity)
        {
            const IndexType newCapacity = CalculateGrowth();
            Reserve(newCapacity);
        }

        for (IndexType i = mSize; i > nIndex; --i)
        {
            GetData()[i] = std::move(GetData()[i - 1]);
        }

        new (GetData() + nIndex) ElementType(std::forward<Args>(vArgs)...);

        return mSize++;
    }

    inline IndexType Add(ElementType &&element) { return Emplace(GetSize(), std::move(element)); }
    inline IndexType Add(const ElementType &element) { return Emplace(GetSize(), element); }

    inline IndexType InsertAt(IndexType nIndex, ElementType &&element) { return Emplace(nIndex, std::move(element)); }
    inline IndexType InsertAt(IndexType nIndex, const ElementType &element) { return Emplace(nIndex, element); }

    inline IndexType AddSlots(IndexType nCount = 1)
    {
        const IndexType newSize = mSize + nCount;
        const IndexType oldSize = mSize;

        Resize(newSize);

        return oldSize;
    }

    inline void Append(const ElementType *pElements, IndexType nSize)
    {
        const IndexType newSize = mSize + nSize;
        const IndexType oldSize = mSize;

        Resize(newSize);

        CMemoryUtils::Copy(GetData() + oldSize, pElements, nSize);
    }

    inline void Append(const TArray &other)
    {
        const IndexType newSize = mSize + other.mSize;
        const IndexType oldSize = mSize;

        Resize(newSize);

        CMemoryUtils::Copy(GetData() + oldSize, other.GetData(), other.GetSize());
    }

    inline ElementType Pop()
    {
        we_assert(!IsEmpty());
        const ElementType element = std::move(GetData()[mSize - 1]);
        --mSize;
        return element;
    }

    inline void Resize(IndexType nNewSize)
    {
        if (nNewSize > mCapacity)
        {
            Reserve(nNewSize);
        }

        if (nNewSize > mSize)
        {
            CMemoryUtils::Construct(GetData() + (usize)mSize, (usize)nNewSize - (usize)mSize);
        }
        else if (nNewSize < mSize)
        {
            CMemoryUtils::Destroy(GetData() + (usize)nNewSize, (usize)mSize - (usize)nNewSize);
        }

        mSize = nNewSize;
    }

    // Stl iterators
    inline Iterator begin() noexcept { return GetData(); }
    NDISCARD inline ConstIterator begin() const noexcept { return GetData(); }

    inline Iterator end() noexcept { return GetData() + mSize; }
    NDISCARD inline ConstIterator end() const noexcept { return GetData() + mSize; }

    inline Iterator rbegin() noexcept { return GetData() + mSize - 1; }
    NDISCARD inline ConstIterator rbegin() const noexcept { return GetData() + mSize - 1; }

    inline Iterator rend() noexcept { return GetData() - 1; }
    NDISCARD inline ConstIterator rend() const noexcept { return GetData() - 1; }

    // Global container functions
    friend inline ElementType *GetData(TArray arr) { return arr.GetData(); }
    friend inline usize GetSize(TArray arr) { return usize(arr.GetSize()); }

    friend inline bool operator==(const TArray &lhs, const TArray &rhs)
    {
        if (lhs.GetSize() != rhs.GetSize())
        {
            return false;
        }

        for (IndexType i = 0; i < lhs.GetSize(); ++i)
        {
            if (lhs[i] != rhs[i])
            {
                return false;
            }
        }

        return true;
    }

  private:
    inline void InitializeWithSize(IndexType nInitialSize)
    {
        mSize = nInitialSize;
        mCapacity = nInitialSize;
        m_Allocator.Reallocate(nInitialSize * sizeof(ElementType));
        CMemoryUtils::Construct(GetData(), nInitialSize);
    }

    inline void InitializeWithRange(const ElementType *pBegin, const ElementType *pEnd)
    {
        const usize size = pEnd - pBegin;
        mSize = IndexType(size);
        mCapacity = IndexType(size);
        m_Allocator.Reallocate(size * sizeof(ElementType));
        CMemoryUtils::Copy(GetData(), pBegin, size);
    }

    inline void CopyFromOther(const TArray &other)
    {
        mSize = other.mSize;
        mCapacity = other.mSize;
        if (mSize > 0)
        {
            m_Allocator.Reallocate(mSize * sizeof(ElementType));
            CMemoryUtils::Copy(GetData(), other.GetData(), mSize);
        }
    }

    inline void MoveFromOther(TArray &&other) noexcept
    {
        mSize = other.mSize;
        mCapacity = other.mCapacity;

        other.m_Allocator.MoveTo(m_Allocator);
        other.mSize = 0;
        other.mCapacity = 0;
    }

    inline IndexType CalculateGrowth() { return mCapacity > 0 ? (mCapacity * 2) : 1; }

    AllocatorType m_Allocator;
    IndexType mSize;
    IndexType mCapacity;
};

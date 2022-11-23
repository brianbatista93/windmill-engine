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

    inline TArray() : m_Allocator(), m_nSize(0), m_nCapacity(0) {}
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
        CMemoryUtils::Destroy(GetData(), m_nSize);
        m_Allocator.ReleaseData();
    }

    inline TArray &operator=(const TArray &other)
    {
        if (this != &other)
        {
            const auto nDifference = CMath::Abs(m_nSize - other.m_nSize);
            CMemoryUtils::Destroy(GetData() + other.m_nSize, nDifference);

            const IndexType sizeInBytes = other.m_nSize * sizeof(ElementType);
            m_Allocator.Reallocate(sizeInBytes);
            m_nSize = other.m_nSize;
            m_nCapacity = other.m_nSize;
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
            m_nSize = other.m_nSize;
            m_nCapacity = other.m_nCapacity;

            other.m_nSize = 0;
            other.m_nCapacity = 0;
        }

        return *this;
    }

    inline void Reserve(IndexType nCapacity)
    {
        if (nCapacity > m_nCapacity)
        {
            m_Allocator.Reallocate(nCapacity * sizeof(ElementType));
            m_nCapacity = nCapacity;
        }
    }

    inline void Clear(bool bShrink = false)
    {
        CMemoryUtils::Destroy(GetData(), m_nSize);
        m_nSize = 0;
        if (bShrink)
        {
            ShrinkToFit();
        }
    }

    inline void ShrinkToFit()
    {
        if (m_nCapacity != m_nSize)
        {
            m_Allocator.Reallocate(m_nSize * sizeof(ElementType));
            m_nCapacity = m_nSize;
        }
    }

    inline IndexType RemoveAt(IndexType nIndex)
    {
        we_assert(nIndex < m_nSize && "Index out of bound");

        for (IndexType i = m_nSize; i > nIndex; --i)
        {
            GetData()[i] = std::move(GetData()[i - 1]);
        }

        return --m_nSize;
    }

    inline bool IsEmpty() const { return GetSize() == 0; }
    inline IndexType GetSize() const { return m_nSize; }
    inline IndexType GetCapacity() const { return m_nCapacity; }

    inline ElementType &At(IndexType nIndex)
    {
        we_assert(nIndex < m_nSize);
        return GetData()[nIndex];
    }
    inline const ElementType &At(IndexType nIndex) const
    {
        we_assert(nIndex < m_nSize);
        return GetData()[nIndex];
    }

    inline ElementType &operator[](IndexType nIndex) { return At(nIndex); }
    inline const ElementType &operator[](IndexType nIndex) const { return At(nIndex); }

    inline ElementType &Front() { return At(0); }
    inline const ElementType &Front() const { return At(0); }

    inline ElementType &Back() { return At(m_nSize - 1); }
    inline const ElementType &Back() const { return At(m_nSize - 1); }

    inline ElementType *GetData() { return (ElementType *)m_Allocator.GetData(); }
    inline const ElementType *GetData() const { return (const ElementType *)m_Allocator.GetData(); }

    template <class... Args>
    inline IndexType Emplace(IndexType nIndex, Args &&...vArgs) noexcept
    {
        we_assert(nIndex <= m_nSize && "Index out of bound");

        const IndexType newSize = m_nSize + 1;
        if (newSize > m_nCapacity)
        {
            const IndexType newCapacity = CalculateGrowth();
            Reserve(newCapacity);
        }

        for (IndexType i = m_nSize; i > nIndex; --i)
        {
            GetData()[i] = std::move(GetData()[i - 1]);
        }

        new (GetData() + nIndex) ElementType(std::forward<Args>(vArgs)...);

        return m_nSize++;
    }

    inline IndexType Add(ElementType &&element) { return Emplace(GetSize(), std::move(element)); }
    inline IndexType Add(const ElementType &element) { return Emplace(GetSize(), element); }

    inline IndexType InsertAt(IndexType nIndex, ElementType &&element) { return Emplace(nIndex, std::move(element)); }
    inline IndexType InsertAt(IndexType nIndex, const ElementType &element) { return Emplace(nIndex, element); }

    inline IndexType AddSlots(IndexType nCount = 1)
    {
        const IndexType newSize = m_nSize + nCount;
        const IndexType oldSize = m_nSize;

        Resize(newSize);

        return oldSize;
    }

    inline void Append(const ElementType *pElements, IndexType nSize)
    {
        const IndexType newSize = m_nSize + nSize;
        const IndexType oldSize = m_nSize;

        Resize(newSize);

        memcpy(GetData() + oldSize, pElements, nSize * sizeof(ElementType));
    }

    inline ElementType Pop()
    {
        we_assert(!IsEmpty());
        ElementType element = std::move(GetData()[m_nSize - 1]);
        --m_nSize;
        return element;
    }

    inline void Resize(IndexType nNewSize)
    {
        if (nNewSize > m_nCapacity)
        {
            Reserve(nNewSize);
        }

        if (nNewSize > m_nSize)
        {
            CMemoryUtils::Construct(GetData() + m_nSize, nNewSize - m_nSize);
        }
        else if (nNewSize < m_nSize)
        {
            CMemoryUtils::Destroy(GetData() + nNewSize, m_nSize - nNewSize);
        }

        m_nSize = nNewSize;
    }

    // Stl iterators
    inline Iterator begin() noexcept { return GetData(); }
    inline ConstIterator begin() const noexcept { return GetData(); }

    inline Iterator end() noexcept { return GetData() + m_nSize; }
    inline ConstIterator end() const noexcept { return GetData() + m_nSize; }

    inline Iterator rbegin() noexcept { return GetData() + m_nSize - 1; }
    inline ConstIterator rbegin() const noexcept { return GetData() + m_nSize - 1; }

    inline Iterator rend() noexcept { return GetData() - 1; }
    inline ConstIterator rend() const noexcept { return GetData() - 1; }

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
        m_nSize = nInitialSize;
        m_nCapacity = nInitialSize;
        m_Allocator.Reallocate(nInitialSize * sizeof(ElementType));
        CMemoryUtils::Construct(GetData(), nInitialSize);
    }

    inline void InitializeWithRange(const ElementType *pBegin, const ElementType *pEnd)
    {
        const usize size = pEnd - pBegin;
        m_nSize = IndexType(size);
        m_nCapacity = IndexType(size);
        m_Allocator.Reallocate(size * sizeof(ElementType));
        CMemoryUtils::Copy(GetData(), pBegin, size);
    }

    inline void CopyFromOther(const TArray &other) noexcept
    {
        m_nSize = other.m_nSize;
        m_nCapacity = other.m_nSize;
        m_Allocator.Reallocate(m_nSize * sizeof(ElementType));
        CMemoryUtils::Copy(GetData(), other.GetData(), m_nSize);
    }

    inline void MoveFromOther(TArray &&other) noexcept
    {
        m_nSize = other.m_nSize;
        m_nCapacity = other.m_nCapacity;

        other.m_Allocator.MoveTo(m_Allocator);
        other.m_nSize = 0;
        other.m_nCapacity = 0;
    }

    inline IndexType CalculateGrowth() { return m_nCapacity > 0 ? (m_nCapacity * 2) : 1; }

    AllocatorType m_Allocator;
    IndexType m_nSize;
    IndexType m_nCapacity;
};
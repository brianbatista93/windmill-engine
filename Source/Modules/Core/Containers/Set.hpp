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

#include <initializer_list>
#include <type_traits>

#include "Array.hpp"
#include "Math/MathUtils.hpp"

template <class T>
struct SSetEntry
{
    u64 Hash{0};
    i64 Next{-1};
    T Value;
};

template <class T, class TSize = usize, class TComparer = std::equal_to<T>>
class TSet
{
  public:
    using ElementType = T;
    using SizeType = TSize;
    using IndexType = std::make_signed_t<TSize>;
    using EntryType = SSetEntry<T>;

    inline TSet() : m_nFreeIndex(-1), m_nCount(0), m_nFreeCount(0) {}

    inline TSet(SizeType nCapacity)
    {
        we_assert(nCapacity > 0);

        Init(nCapacity);
    }

    inline TSet(std::initializer_list<ElementType> list)
    {
        Init(list.size());

        for (auto value : list)
        {
            Add(value);
        }
    }

    inline bool IsEmpty() const { return m_nCount == 0; }

    inline SizeType GetCount() const { return m_nCount - m_nFreeCount; }

    inline bool Add(ElementType &&element) { return AddEntry(std::forward<ElementType>(element), nullptr); }
    inline bool Add(const ElementType &element) { return AddEntry(element, nullptr); }

    inline bool Remove(const ElementType &element)
    {
        if (!m_Buckets.IsEmpty())
        {
            const u64 hash = GetHash(element);
            i64 *bucket = m_Buckets.begin() + (hash % m_Buckets.GetSize());
            i64 i = *bucket - 1;
            i64 last = -1;
            u64 collision_count_ = 0;

            while (i >= 0)
            {
                EntryType &entry = m_Entries[i];
                if (entry.Hash == hash and m_Comparer(entry.Value, element))
                {
                    if (last < 0)
                    {
                        *bucket = entry.Next + 1;
                    }
                    else
                    {
                        m_Entries[last].Next = entry.Next;
                    }

                    we_assert((-3 - m_nFreeIndex) < 0);
                    entry.Next = -3 - m_nFreeIndex;

                    if constexpr (std::is_reference_v<T>)
                    {
                        entry.Value = T();
                    }
                    else
                    {
                        entry.Value.~T();
                    }

                    m_nFreeIndex = i;
                    m_nFreeCount++;
                    return true;
                }

                last = i;
                i = entry.Next;
                collision_count_++;
                we_assert(collision_count_ <= m_Entries.GetSize());
            }
        }

        return false;
    }

    inline void Clear(bool bShrink = false)
    {
        m_Entries.Clear(bShrink);
        m_Buckets.Clear(bShrink);
        m_nFreeIndex = -1;
        m_nCount = 0;
        m_nFreeCount = 0;
    }

    inline const ElementType *Find(const T &element) const
    {
        if (!m_Buckets.IsEmpty())
        {
            const u64 hash = GetHash(element);
            const i64 *bucket = m_Buckets.begin() + (hash % m_Buckets.GetSize());
            i64 i = *bucket - 1;
            u64 collision_count_ = 0;

            while (i >= 0)
            {
                const EntryType *entry = (m_Entries.GetData() + i);
                if (entry->Hash == hash and m_Comparer(entry->Value, element))
                {
                    return &entry->Value;
                }

                i = entry->Next;
                collision_count_++;
                we_assert(collision_count_ <= m_Entries.GetSize());
            }
        }

        return nullptr;
    }

    inline bool Contains(const T &element) const { return Find(element) != nullptr; }

  private:
    inline void Init(SizeType nCapacity)
    {
        const SizeType size = CMath::GetPrime(nCapacity);

        m_Entries.Resize(size);
        m_Buckets.Resize(size);
        m_nFreeIndex = -1;
        m_nCount = 0;
        m_nFreeCount = 0;
    }

    inline void Resize(SizeType nNewSize)
    {
        we_assert(nNewSize >= m_Entries.GetSize());

        m_Entries.Resize(nNewSize);
        m_Buckets.Resize(nNewSize);

        for (SizeType i = 0; i < m_nCount; i++)
        {
            EntryType &entry = m_Entries[i];
            if (entry.Next >= -1)
            {
                i64 &bucket = m_Buckets[entry.Hash % m_Buckets.GetSize()];
                entry.Next = bucket - 1;
                bucket = i + 1;
            }
        }
    }

    template <class U>
    inline bool AddEntry(typename std::type_identity_t<U> value, SizeType *oIndex)
    {
        if (m_Buckets.IsEmpty())
        {
            Init(1);
        }

        u32 collision_count = 0;
        const u64 hash = GetHash(value);
        i64 *bucket = m_Buckets.begin() + (hash % m_Buckets.GetSize());
        i64 i = *bucket - 1;

        while (i >= 0)
        {
            EntryType &entry = m_Entries[i];
            if (entry.Hash == hash and m_Comparer(entry.Value, value))
            {
                set_if_not_null(oIndex, i);
                return false;
            }
            i = entry.Next;

            collision_count++;
            we_assert(collision_count <= m_Entries.GetSize());
        }

        i64 index;
        if (m_nFreeIndex > 0)
        {
            index = m_nFreeIndex--;
            we_assert((-3 - m_Entries[m_nFreeIndex].Next) >= -1);
            m_nFreeIndex = -3 - m_Entries[m_nFreeIndex].Next;
        }
        else
        {
            size_t count = m_nCount;
            if (count == m_Entries.GetSize())
            {
                Resize();
                bucket = m_Buckets.begin() + (hash % m_Buckets.GetSize());
            }
            index = count;
            m_nCount = count + 1;
        }

        {
            EntryType &entry = m_Entries[index];
            entry.Hash = hash;
            entry.Next = *bucket - 1;
            entry.Value = std::move(value);
            *bucket = index + 1;
            set_if_not_null(oIndex, static_cast<size_t>(index));
        }

        return true;
    }

    TArray<EntryType, TAllocator<IndexType>> m_Entries;
    TArray<IndexType, TAllocator<IndexType>> m_Buckets;
    TComparer m_Comparer;
    IndexType m_nFreeIndex;
    TSize m_nCount;
    TSize m_nFreeCount;
};
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

template <class Type>
struct SSetEntry
{
    u64 Hash{0};
    i64 Next{-1};
    Type Value;
};

template <class Type, class SizeType = usize, class ComparerType = std::equal_to<Type>>
class CTSet
{
  public:
    using ElementType = Type;
    using IndexType = std::make_signed_t<SizeType>;
    using EntryType = SSetEntry<Type>;

    inline CTSet() : mFreeIndex(-1), mCount(0), mFreeCount(0) {}

    inline CTSet(SizeType nCapacity)
    {
        we_assert(nCapacity > 0);

        Init(nCapacity);
    }

    inline CTSet(std::initializer_list<ElementType> list)
    {
        Init(list.size());

        for (auto value : list)
        {
            Add(value);
        }
    }

    inline bool IsEmpty() const { return mCount == 0; }

    inline SizeType GetCount() const { return mCount - mFreeCount; }

    inline bool Add(ElementType &&element) { return AddEntry(std::forward<ElementType>(element), nullptr); }
    inline bool Add(const ElementType &element) { return AddEntry(element, nullptr); }

    inline bool Remove(const ElementType &element)
    {
        if (!mBuckets.IsEmpty())
        {
            const u64 hash = GetHash(element);
            i64 *bucket = mBuckets.begin() + (hash % mBuckets.GetSize());
            i64 i = *bucket - 1;
            i64 last = -1;
            u64 collision_count_ = 0;

            while (i >= 0)
            {
                EntryType &entry = mEntries[i];
                if (entry.Hash == hash and m_Comparer(entry.Value, element))
                {
                    if (last < 0)
                    {
                        *bucket = entry.Next + 1;
                    }
                    else
                    {
                        mEntries[last].Next = entry.Next;
                    }

                    we_assert((-3 - mFreeIndex) < 0);
                    entry.Next = -3 - mFreeIndex;

                    if constexpr (std::is_reference_v<Type>)
                    {
                        entry.Value = Type();
                    }
                    else
                    {
                        entry.Value.~Type();
                    }

                    mFreeIndex = i;
                    mFreeCount++;
                    return true;
                }

                last = i;
                i = entry.Next;
                collision_count_++;
                we_assert(collision_count_ <= mEntries.GetSize());
            }
        }

        return false;
    }

    inline void Clear(bool bShrink = false)
    {
        mEntries.Clear(bShrink);
        mBuckets.Clear(bShrink);
        mFreeIndex = -1;
        mCount = 0;
        mFreeCount = 0;
    }

    inline const ElementType *Find(const Type &element) const
    {
        if (!mBuckets.IsEmpty())
        {
            const u64 hash = GetHash(element);
            const i64 *bucket = mBuckets.begin() + (hash % mBuckets.GetSize());
            i64 i = *bucket - 1;
            u64 collision_count_ = 0;

            while (i >= 0)
            {
                const EntryType *entry = (mEntries.GetData() + i);
                if (entry->Hash == hash and m_Comparer(entry->Value, element))
                {
                    return &entry->Value;
                }

                i = entry->Next;
                collision_count_++;
                we_assert(collision_count_ <= mEntries.GetSize());
            }
        }

        return nullptr;
    }

    inline bool Contains(const Type &element) const { return Find(element) != nullptr; }

  private:
    inline void Init(SizeType nCapacity)
    {
        const SizeType size = CMath::GetPrime(nCapacity);

        mEntries.Resize(size);
        mBuckets.Resize(size);
        mFreeIndex = -1;
        mCount = 0;
        mFreeCount = 0;
    }

    inline void Resize(SizeType nNewSize)
    {
        we_assert(nNewSize >= mEntries.GetSize());

        mEntries.Resize(nNewSize);
        mBuckets.Resize(nNewSize);

        for (SizeType i = 0; i < mCount; i++)
        {
            EntryType &entry = mEntries[i];
            if (entry.Next >= -1)
            {
                i64 &bucket = mBuckets[entry.Hash % mBuckets.GetSize()];
                entry.Next = bucket - 1;
                bucket = i + 1;
            }
        }
    }

    template <class UType>
    inline bool AddEntry(typename std::type_identity_t /*unused*/<UType> value, SizeType *oIndex)
    {
        if (mBuckets.IsEmpty())
        {
            Init(1);
        }

        u32 collision_count = 0;
        const u64 hash = GetHash(value);
        i64 *bucket = mBuckets.begin() + (hash % mBuckets.GetSize());
        i64 i = *bucket - 1;

        while (i >= 0)
        {
            EntryType &entry = mEntries[i];
            if (entry.Hash == hash and m_Comparer(entry.Value, value))
            {
                SetIfNotNull(oIndex, i);
                return false;
            }
            i = entry.Next;

            collision_count++;
            we_assert(collision_count <= mEntries.GetSize());
        }

        i64 index;
        if (mFreeIndex > 0)
        {
            index = mFreeIndex--;
            we_assert((-3 - mEntries[mFreeIndex].Next) >= -1);
            mFreeIndex = -3 - mEntries[mFreeIndex].Next;
        }
        else
        {
            size_t count = mCount;
            if (count == mEntries.GetSize())
            {
                Resize();
                bucket = mBuckets.begin() + (hash % mBuckets.GetSize());
            }
            index = count;
            mCount = count + 1;
        }

        {
            EntryType &entry = mEntries[index];
            entry.Hash = hash;
            entry.Next = *bucket - 1;
            entry.Value = std::move(value);
            *bucket = index + 1;
            SetIfNotNull(oIndex, static_cast<size_t>(index));
        }

        return true;
    }

    CArray<EntryType, CAllocator<IndexType>> mEntries;
    CArray<IndexType, CAllocator<IndexType>> mBuckets;
    ComparerType mComparer;
    IndexType mFreeIndex;
    SizeType mCount;
    SizeType mFreeCount;
};

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

#include "Set.hpp"

template <class KeyType, class ValueType>
class CMapEqualTo
{
  public:
    inline bool operator()(const std::pair<KeyType, ValueType> &lhs, const std::pair<KeyType, ValueType> &rhs) const noexcept
    {
        return lhs.first == rhs.first;
    }
};

template <class KeyType, class ValueType, class ComparerType = CMapEqualTo<KeyType, ValueType>>
class CMap
{
  public:
    using PairType = typename std::pair<KeyType, ValueType>;
    using SetType = CSet<PairType, usize, ComparerType>;

    CMap() = default;
    CMap(CMap &&) noexcept = default;
    CMap(const CMap &) = default;
    CMap &operator=(CMap &&) noexcept = default;
    CMap &operator=(const CMap &) = default;

    ValueType *Find(const KeyType &key)
    {
        auto *iter = mSet.Find(std::make_pair(key, ValueType()));
        if (iter != nullptr)
        {
            return &iter->second;
        }

        return nullptr;
    }

    const ValueType *Find(const KeyType &key) const { return const_cast<CMap *>(this)->Find(key); }

    bool Add(KeyType &&key, ValueType &&value) { return mSet.Add(std::forward<PairType>(std::make_pair(key, value))); }
    bool Add(KeyType &&key, const ValueType &value) { return mSet.Add(std::forward<PairType>(std::make_pair(key, value))); }
    bool Add(const KeyType &key, ValueType &&value) { return mSet.Add(std::forward<PairType>(std::make_pair(key, value))); }
    bool Add(const KeyType &key, const ValueType &value) { return mSet.Add(std::forward<PairType>(std::make_pair(key, value))); }

  private:
    SetType mSet;
};

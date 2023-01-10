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

#include <concepts>

#include "Types.hpp"

// clang-format off
// NOLINTBEGIN

namespace WE::Concept
{
template <class Type>
concept IsContainer = requires(Type lhs, const Type& rhs)
{
    { lhs.begin() } -> std::same_as<typename Type::iterator>;
    { lhs.end() } -> std::same_as<typename Type::iterator>;
    { rhs.begin() } -> std::same_as<typename Type::const_iterator>;
    { rhs.end() } -> std::same_as<typename Type::const_iterator>;
};

template <class Type>
concept IsFormattable = requires(Type value)
{
    { value.TryFormat(value, nullptr, nullptr) } -> std::same_as<bool>;
};

template <class Type>
concept IsNumeric = (std::integral <Type> || std::floating_point <Type>) && !std::same_as <Type, bool>;
} // namespace WE::Concept

// NOLINTEND
// clang-format on

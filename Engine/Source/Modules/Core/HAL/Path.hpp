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

#include "Containers/WeString.hpp"

class CPath
{
  public:
    inline static tchar kSeparator = WT('/');

    CPath() = default;
    CPath(const CPath &) = default;
    CPath(CPath &&) noexcept = default;
    ~CPath() = default;
    CPath &operator=(const CPath &) = default;
    CPath &operator=(CPath &&) noexcept = default;

    CPath(CString &&source) : m_Text(std::move(source)) { PreprocessPath(); }

    explicit CPath(const tchar *pStr) : m_Text(pStr) { PreprocessPath(); }

    CPath &operator=(CString &&source) noexcept
    {
        m_Text = std::move(source);
        PreprocessPath();
        return *this;
    }

    inline const CString::CharType *operator*() const { return *m_Text; }

    [[nodiscard]] inline bool IsEmpty() const { return m_Text.IsEmpty(); }
    [[nodiscard]] inline i32 GetLength() const { return m_Text.GetLength(); }
    [[nodiscard]] bool IsDirectory() const;
    [[nodiscard]] bool IsFile() const;

    inline const CString ToString() const { return m_Text; }

    TArray<CPath> GetAllFiles(const tchar *pFilter, bool bRecursive = false) const;

    [[nodiscard]] inline CPath GetParentPath() const
    {
        we_assert(!IsEmpty());
        auto it = std::find_if(m_Text.rbegin(), m_Text.rend(), [](tchar c) { return c == kSeparator; });
        usize length = std::distance(it, m_Text.rend());
        we_assert((i32)length <= m_Text.GetLength());
        return CPath(CString(*m_Text, length - 1));
    }

    [[nodiscard]] inline CPath GetFileName() const
    {
        we_assert(!IsEmpty());
        auto it = std::find_if(m_Text.rbegin(), m_Text.rend(), [](tchar c) { return c == kSeparator; });
        return CPath(CString(it.base()));
    }

    [[nodiscard]] inline CPath GetFileNameWithoutExtension() const
    {
        we_assert(!IsEmpty());
        auto itExt = std::find_if(m_Text.rbegin(), m_Text.rend(), [](tchar c) { return c == '.'; });
        auto it = std::find_if(itExt, m_Text.rend(), [](tchar c) { return c == kSeparator; });
        return CPath(CString(it.base(), itExt.base()));
    }

    friend CPath operator/(const CPath &lhs, const CPath &rhs)
    {
        CPath result(lhs);
        return result.Append(rhs);
    }

    friend CPath operator/(const CPath &lhs, const tchar *pRhs)
    {
        CPath result(lhs);
        return result.Append(pRhs, std::char_traits<tchar>::length(pRhs));
    }

    friend CPath operator+(const CPath &lhs, const tchar *pStr) { return lhs.m_Text + pStr; }
    CPath &operator+=(const tchar *pStr)
    {
        m_Text.Append(pStr);
        return *this;
    }

    CPath &Append(const tchar *pStr, i32 nLength);
    CPath &Append(const CPath &other) { return Append(*other.m_Text, other.m_Text.GetLength()); }

  private:
    void PreprocessPath() { std::replace(m_Text.begin(), m_Text.end(), WT('\\'), kSeparator); }

    CString m_Text;
};

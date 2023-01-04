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

#include "Containers/Map.hpp"
#include "Containers/StringView.hpp"
#include "HAL/Path.hpp"
#include "Text/NumericParser.hpp"

class CSection : public CMap<CString, CString>
{
};

class CConfigFile : public CMap<CString, CSection>
{
  public:
    static constexpr tchar kCommentCharacter = ';';
    static constexpr tchar kAssignmentCharacter = '=';
    static constexpr tchar kBeginSectionCharacter = '[';
    static constexpr tchar kEndSectionCharacter = ']';
    static constexpr tchar kSectionKeySeparator = '.';
    static constexpr const tchar *kGlobalSection = WT("$global");

    CConfigFile() = default;

    CConfigFile(CConfigFile &&) noexcept = default;

    CConfigFile(const CConfigFile &) = default;

    CConfigFile &operator=(CConfigFile &&) noexcept = default;

    CConfigFile &operator=(const CConfigFile &) = default;

    bool Load(const CPath &fileName);
    bool Save() const;

    // TODO: GetValueAs
    const CSection *GetSection(const CString &section) const { return Find(section); }

    template <class Type>
    Type GetValueAs(const tchar *pSectionKey, Type defaultValue = {}) const
    {
        we_assert(pSectionKey && *pSectionKey);

        CString section;
        CString key;

        i32 separator = CStringUtils::Find(pSectionKey, kSectionKeySeparator);
        if (separator != -1)
        {
            section = CStringView{pSectionKey}.Substring(0, separator).ToString();
            key = CStringView{pSectionKey}.Substring(separator + 1).ToString();
        }
        else if (!key.IsEmpty())
        {
            section = kGlobalSection;
        }
        else
        {
            return defaultValue;
        }

        return GetValueAs<Type>(*section, *key, defaultValue);
    }

    template <class Type>
    Type GetValueAs(const tchar *pSection, const tchar *pKey, Type defaultValue = {}) const
    {
        we_assert(pKey && *pKey);

        if (const auto *section = GetSection(pSection))
        {
            if (const auto *value = section->Find(pKey))
            {
                Type result{};
                if (TryParse(**value, &result))
                {
                    return result;
                }
            }
        }

        return defaultValue;
    }

  private:
    bool Parse(const CStringView &str);

    static void RemoveComments(CStringView &line);
    static void RemoveLeftWhiteTrails(CStringView &line);
    static void RemoveRightWhiteTrails(CStringView &line);
    static void RemoveWhiteTrails(CStringView &line);
    static bool ParseSection(const CStringView &line, i32 nLine, CString &section);
    static bool ParseKeyValue(const CStringView &line, i32 nLine, CString &key, CString &value);

    bool AddSectionKeyValue(CString section, CString &&key, CString &&value);

    CPath mPath;
};

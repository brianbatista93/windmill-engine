#include "ConfigFile.hpp"
#include "HAL/File.hpp"
#include "Iterators/LineIterator.hpp"
#include "Logging/Log.hpp"

DECLARE_STATIC_LOG_EMITTER(ConfigFile, eInfo);

bool CConfigFile::Load(const CPath &fileName)
{
    CString content;
    if (!CFile::ReadString(content, fileName))
    {
        return false;
    }

    if (!Parse(content))
    {
        return false;
    }

    mPath = fileName;

    return true;
}

bool CConfigFile::Save() const
{
    return mPath.IsEmpty();
}

bool CConfigFile::Parse(const CStringView &str)
{
    CLineIterator lineIterator(str);

    CStringView line;
    i32 lineNumber = 0;
    while (lineIterator.GetLine(line))
    {
        ++lineNumber;

        RemoveWhiteTrails(line);
        RemoveComments(line);
        if (line.IsEmpty())
        {
            continue;
        }

        static CString section;
        CString key;
        CString value;
        if (line[0] == kBeginSectionCharacter)
        {
            if (!ParseSection(line, lineNumber, section))
            {
                return false;
            }

            continue;
        }

        if (!ParseKeyValue(line, lineNumber, key, value))
        {
            return false;
        }

        if (!AddSectionKeyValue(section, std::forward<CString>(key), std::forward<CString>(value)))
        {
            return false;
        }
    }

    return true;
}

void CConfigFile::RemoveComments(CStringView &line)
{
    if (!line.IsEmpty())
    {
        line.Remove(line.Find(kCommentCharacter));
    }
}

void CConfigFile::RemoveLeftWhiteTrails(CStringView &line)
{
    tchar *begin = line.begin();
    tchar *end = line.end();

    for (; begin != end; ++begin)
    {
        if (!iswspace(*begin))
        {
            break;
        }
    }

    line = {begin, end};
}

void CConfigFile::RemoveRightWhiteTrails(CStringView &line)
{
    tchar *begin = line.end();
    tchar *end = line.begin();

    for (; begin != end; --begin)
    {
        if (!iswspace(*begin))
        {
            break;
        }
    }

    line = {end, begin + 1};
}

void CConfigFile::RemoveWhiteTrails(CStringView &line)
{
    if (!line.IsEmpty())
    {
        RemoveLeftWhiteTrails(line);
        RemoveRightWhiteTrails(line);
    }
}

bool CConfigFile::ParseSection(const CStringView &line, i32 nLine, CString &section)
{
    if (line[0] == kBeginSectionCharacter && line[line.GetLength() - 1] != kEndSectionCharacter)
    {
        WE_ERROR(ConfigFile, WT("Invalid section name in line {0}"), nLine);
        return false;
    }

    if (line[0] == kBeginSectionCharacter && line[line.GetLength() - 1] == kEndSectionCharacter)
    {
        const CStringView sectionView = line.Substring(1, line.GetLength() - 2);
        if (sectionView.IsEmpty())
        {
            WE_ERROR(ConfigFile, WT("Empty section name in line {0}"), nLine);
            return false;
        }

        section = sectionView.ToString();

        return true;
    }

    return true;
}

bool CConfigFile::ParseKeyValue(const CStringView &line, i32 nLine, CString &key, CString &value)
{
    if (line.IsEmpty())
    {
        return false;
    }

    const i32 assignPosition = line.Find(kAssignmentCharacter);
    if (assignPosition == -1)
    {
        WE_ERROR(ConfigFile, WT("Could not find assignment character in line {0}"), nLine);
        return false;
    }

    CStringView keyView = line.Substring(0, assignPosition - 1);
    CStringView valueView = line.Substring(assignPosition + 1, line.GetLength() - assignPosition - 1);

    RemoveWhiteTrails(keyView);
    RemoveWhiteTrails(valueView);

    if (keyView.IsEmpty())
    {
        WE_ERROR(ConfigFile, WT("Could not find key in line {0}"), nLine);
        return false;
    }

    if (valueView.IsEmpty())
    {
        WE_ERROR(ConfigFile, WT("Could not find value in line {0}"), nLine);
        return false;
    }

    key = keyView.ToString();
    value = valueView.ToString();

    return true;
}

bool CConfigFile::AddSectionKeyValue(CString section, CString &&key, CString &&value)
{
    if (section.IsEmpty())
    {
        section = kGlobalSection;
    }

    CSection *sectionPointer = Find(section);

    if (sectionPointer == nullptr)
    {
        Add(section, {});
        sectionPointer = Find(section);
    }

    const CString *keyValue = sectionPointer->Find(key);
    if (keyValue != nullptr)
    {
        WE_ERROR(ConfigFile, WT("Key {0} already exists in section {1}"), key, section);
        return false;
    }

    sectionPointer->Add(std::move(key), std::move(value));
    return true;
}

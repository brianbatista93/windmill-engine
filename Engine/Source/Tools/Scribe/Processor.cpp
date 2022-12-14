#include "Attributes.hpp"
#include "HAL/File.hpp"
#include "HAL/FileSystem.hpp"
#include "Logging/Log.hpp"

DECLARE_STATIC_LOG_EMITTER(Scribe, eInfo);

CPath gLicenseFile; // NOLINT

bool Parse(const CString &content, CStringBuilder &builder);

CStringBuilder WriteHeaderAndGetStringBuilder();

tchar *IgnoreWhiteSpace(const tchar *pStr, i32 &rLine);
tchar *IgnoreComments(const tchar *pStr, i32 &rLine);
tchar *ParseDeep(CStringBuilder &builder, const tchar *pStr, i32 &rLine);
tchar *IdentifyAttribute(CStringBuilder &builder, const tchar *pStr, i32 &rLine);
tchar *ProcessAttribute(CStringBuilder &builder, const tchar *pStr, i32 &rLine);

CString GetAttributeName(const tchar * /*attributeName*/);

bool ProcessFile(const CPath &filePath, const CPath &output)
{
    WE_INFO(Scribe, WT("Precomputing file: {0}"), *filePath);

    if (!CFile::Exist(filePath))
    {
        WE_ERROR(Scribe, WT("File {0} does not exist"), *filePath);
        return false;
    }

    if (!CFileSystem::DirectoryExists(output))
    {
        if (!CFileSystem::CreateDirectory(output))
        {
            return false;
        }
    }

    CString fileContent = {};
    if (!CFile::ReadString(fileContent, filePath))
    {
        WE_ERROR(Scribe, WT("Failed to read file {0}"), *filePath);
        return false;
    }

    CStringBuilder builder{WriteHeaderAndGetStringBuilder()};

    if (!Parse(fileContent, builder))
    {
        WE_ERROR(Scribe, WT("Failed to parse file {0}"), *filePath);
        return false;
    }

    if (!CFile::WriteString(builder.ToString(), output / filePath.GetFileNameWithoutExtension() + WT(".gen.hpp")))
    {
        WE_ERROR(Scribe, WT("Failed to write file {0}"), *filePath);
        return false;
    }

    return true;
}

bool Parse(const CString &content, CStringBuilder &builder)
{
    if (content.IsEmpty())
    {
        return true;
    }

    i32 line = 1;
    auto *pPointer = (tchar *)*content;
    pPointer = IgnoreWhiteSpace(pPointer, line);

    return ParseDeep(builder, pPointer, line);
}

CStringBuilder WriteHeaderAndGetStringBuilder()
{
    CStringBuilder builder(1024);

    CString license;
    CFile::ReadString(license, gLicenseFile);

    if (!license.IsEmpty())
    {
        builder.AppendLine(WTL("/*"));
        builder.Append(license);
        builder.AppendLine(WTL("*/"));
    }

    builder.AppendLine(WTL(""));
    builder.AppendLine(WTL("/******************************************************/"));
    builder.AppendLine(WTL("/* This file was generated by Scribe. Don't modify it.*/"));
    builder.AppendLine(WTL("/******************************************************/"));
    builder.AppendLine(WTL(""));
    builder.AppendLine(WTL("#pragma once"));

    return builder;
}

tchar *IgnoreWhiteSpace(const tchar *pStr, i32 &rLine)
{
    we_assert(pStr);

    while (isspace(*pStr))
    {
        if (*pStr == WT('\n'))
        {
            ++rLine;
        }
        ++pStr;
    }

    we_assert(pStr);
    return (tchar *)pStr;
}

tchar *IgnoreComments(const tchar *pStr, i32 &rLine)
{
    we_assert(pStr and *pStr);

    while (*pStr)
    {
        if (*pStr == WT('/'))
        {
            if (*(pStr + 1) == WT('/'))
            {
                while (*pStr != WT('\n'))
                {
                    ++pStr;
                }
                ++rLine;
                ++pStr;
            }
            else if (*(pStr + 1) == WT('*'))
            {
                while (*pStr != WT('*') or *(pStr + 1) != WT('/'))
                {
                    if (*pStr == WT('\n'))
                    {
                        ++rLine;
                    }
                    ++pStr;
                }
                pStr += 2;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return (tchar *)pStr;
}

tchar *ParseDeep(CStringBuilder &builder, const tchar *pStr, i32 &rLine)
{
    while (pStr and *pStr)
    {
        pStr = IdentifyAttribute(builder, pStr, rLine);
    }

    return (tchar *)pStr;
}

tchar *IdentifyAttribute(CStringBuilder &builder, const tchar *pStr, i32 &rLine)
{
    we_assert(pStr and *pStr);

    pStr = IgnoreWhiteSpace(pStr, rLine);
    pStr = IgnoreComments(pStr, rLine);

    static const tchar *sAttrBegin = {WT("[[WE::")};
    // static const tchar *sAttrEnd = {WT("]]")};

    constexpr static i32 kAttrBeginLength = 6;
    // static i32 sAttrEndLength = 2;

    while (pStr and *pStr)
    {
        if (CStringUtils::Equal(pStr, sAttrBegin, kAttrBeginLength))
        {
            pStr += kAttrBeginLength;
            pStr = ProcessAttribute(builder, pStr, rLine);
        }
        else if (*pStr == WT('\n'))
        {
            ++rLine;
        }

        ++pStr;
    }

    return (tchar *)pStr;
}

tchar *ProcessAttribute(CStringBuilder &builder, const tchar *pStr, i32 &rLine)
{
    const static CAttributeManager kAttributeManager;

    const CString attributeName = GetAttributeName(pStr);
    if (auto attribute = kAttributeManager.FindAttribute(*attributeName); attribute != nullptr)
    {
        return attribute(builder, pStr, rLine);
    }

    // TODO: Fix the attribute name
    WE_ERROR(Scribe, WT("Could not find attribute: {0}"), *attributeName);
    return nullptr;
}

CString GetAttributeName(const tchar *pStr)
{
    const tchar *start = pStr;

    while (*pStr and isalpha(*pStr))
    {
        ++pStr;
    }

    return {start, (usize)(pStr - start)};
}

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
#include "HAL/Path.hpp"
#include "Misc/ULID.hpp"
#include "Types.hpp"

// clang-format off
constexpr const tchar *kProjectDirectories[] = {
    WT("Assets"), // Assets like meshes, audios, etc
	WT("Config"), // Where all config files are stored
	WT("Intermediate"), // Where all intermediate files are stored
	WT("MetaFiles"), // Where all meta files (saved) are stored
	WT("MetaFiles/Logs"), // Where all log files are stored
	WT("MetaFiles/Assets"), // Assets meta informations
	WT("Source"), // Where all source files are stored
	WT("Source/${PROJECT_NAME}"), // Where all source files are stored	
	WT("Source/${PROJECT_NAME}/Classes"), // Where all source files are stored	
};
// clang-format on

constexpr const tchar *kProjectFilename = WT("Project.yml");

class CBailiffProject
{
  public:
    static u32 kProjectFileVersion;

    static void Create(tchar *ppArgs[], i32 nArgC);

  private:
    static bool CreateProjectFile(const CPath &projectDir, const CString &projectName);

    static bool CreateGitIgnoreFile(const CPath &projectDir);

    static CString ExpandString(const tchar *pStr);

    static CMap<CString, CString> kProjectVariables;
};

struct SBailiffProject
{
    const u32 FileVersion{CBailiffProject::kProjectFileVersion};
    u32 EngineVersion;
    tchar Name[32];
    tchar Description[256];
    tchar Author[32];
    CULID ProjectID;
};

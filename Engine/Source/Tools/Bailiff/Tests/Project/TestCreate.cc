#include <gtest/gtest.h>

#include "HAL/FileSystem.hpp"
#include "Project/BailiffProject.hpp"

TEST(Project, Create)
{
    for (i32 i = 0; i < GetArrayCount(kProjectDirectories); ++i)
    {
        EXPECT_TRUE(CFileSystem::DirectoryExists(CPath{kProjectDirectories[i]})) << "Directory " << kProjectDirectories[i] << " does not exist";
    }

    EXPECT_TRUE(CFileSystem::FileExists(CPath{kProjectFilename})) << "File " << kProjectFilename << " does not exist";
}

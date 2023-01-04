#include "LogFileSink.hpp"
#include "HAL/File.hpp"
#include "HAL/FileSystem.hpp"
#include "LogEmitter.hpp"
#include "LogMessage.hpp"

CLogFileSink::CLogFileSink(const CPath &filename)
{
    mFile = CFileSystem::OpenWrite(filename, false, true);
}

void CLogFileSink::Log(const SLogMessage *pMessage)
{
    CStringBuilder builder;
    builder.AppendFormat(WT("{0} [{1}] --- "), *pMessage->pEmitter->GetName(), ToString(pMessage->LogLevel));
    builder.AppendLine(pMessage->FormattedMessage);

    CFile::WriteString(mFile, builder.ToString(), CFile::EEncoding::eUTF8NoBOM);
}

CLogFileSink ::~CLogFileSink()
{
    if (mFile)
    {
        we_delete(mFile);
        mFile = nullptr;
    }
}

#include "LogFileSink.hpp"
#include "HAL/File.hpp"
#include "HAL/FileSystem.hpp"
#include "LogMessage.hpp"
#include "LogEmitter.hpp"

CLogFileSink::CLogFileSink(const CPath &filename)
{
    m_pFile = CFileSystem::OpenWrite(filename, false, true);
}

void CLogFileSink::Log(const SLogMessage *pMessage)
{
    CStringBuilder builder;
    builder.AppendFormat(WT("{0} [{1}] --- "), *pMessage->pEmitter->GetName(), ToString(pMessage->LogLevel));
    builder.AppendLine(pMessage->FormattedMessage);

    CFile::WriteString(m_pFile, builder.ToString(), CFile::EEncoding::eUTF8NoBOM);
}

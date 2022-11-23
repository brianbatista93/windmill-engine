#include "Path.hpp"

CPath &CPath::Append(const tchar *pStr, i32 nLength)
{
    i32 dataCount = m_Text.m_Data.GetSize();
    if (nLength == 0)
    {
        if (dataCount > 1 and m_Text.m_Data[dataCount - 2] != WT('/') and m_Text.m_Data[dataCount - 2] != WT('\\'))
        {
            m_Text.m_Data[dataCount - 1] = WT('/');
            m_Text.m_Data.Add(WT('\0'));
        }
    }
    else
    {
        if (dataCount > 0)
        {
            if (dataCount > 1 and m_Text.m_Data[dataCount - 2] != WT('/') and m_Text.m_Data[dataCount - 2] != WT('\\') and *pStr != WT('/'))
            {
                m_Text.m_Data[dataCount - 1] = WT('/');
            }
            else
            {
                m_Text.m_Data.Pop();
                --dataCount;
            }
        }

        m_Text.m_Data.Reserve(dataCount + nLength);
        m_Text.m_Data.Append(pStr, nLength);
        m_Text.m_Data.Add(WT('\0'));
    }

    return *this;
}

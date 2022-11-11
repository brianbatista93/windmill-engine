#include "StringFormatter.hpp"
#include "WeString.hpp"

CFormatterArgument::CFormatterArgument(const tchar *pValue) : m_eType(kString), m_pString(pValue)
{
}

CFormatterArgument::CFormatterArgument(CString value) : m_eType(kString), m_pString(*value)
{
}

#include "StringFormatter.hpp"
#include "WeString.hpp"

CFormatterArgument::CFormatterArgument(const tchar *pValue) : mType(kString), mString(pValue)
{
}

CFormatterArgument::CFormatterArgument(const CString &value) : mType(kString), mString(*value)
{
}

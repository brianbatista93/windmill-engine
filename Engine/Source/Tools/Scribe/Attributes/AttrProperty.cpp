#include "AttrProperty.hpp"

tchar *ProcessProperty(CStringBuilder & /*unused*/, const tchar *pStr, i32 & /*unused*/)
{
    pStr += sAttrPropertyLength;

    return (tchar *)pStr;
}

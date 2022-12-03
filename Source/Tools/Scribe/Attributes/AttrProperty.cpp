#include "AttrProperty.hpp"

tchar *ProcessProperty(const tchar *pStr, CStringBuilder &, i32 &)
{
    pStr += sAttrPropertyLength;

    return (tchar *)pStr;
}
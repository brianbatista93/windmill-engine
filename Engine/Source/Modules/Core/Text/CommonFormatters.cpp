#include <cstring>

#include "CommonFormatters.hpp"
#include "Containers/StringUtils.hpp"
#include "Encoding/UTF8Encoder.hpp"

bool TryFormat(const char *pValue, tchar **pDest, const tchar *pFormat)
{
    if (pValue && *pValue && pDest)
    {
        const usize length = strlen(pValue);
        CUTF8Encoder::Decode(*pDest, (const u8 *)pValue, length);

        if (pFormat)
        {
            if (*pFormat == WT('s'))
            {
                CStringUtils::ToLower(pDest, *pDest, length);
            }
            else if (*pFormat == WT('S'))
            {
                CStringUtils::ToUpper(pDest, *pDest, length);
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

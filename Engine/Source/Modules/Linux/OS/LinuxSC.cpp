#include <cwchar>
#include <cwctype>

#include "OS/StringConvertion.hpp"

usize OS::TCharToUTF8(utf8 *pDst, const tchar *pSrc, usize nSrcLength)
{
#ifdef UNICODE
    mbstate_t state = {};
    return wcsrtombs(reinterpret_cast<ansi *>(pDst), &pSrc, nSrcLength, &state);
#else
    return 0;
#endif // UNICODE
}

usize OS::UTF8ToTChar(tchar *pDst, const utf8 *pSrc, usize nSrcLength)
{
#ifdef UNICODE
    mbstate_t state = {};
    return mbsrtowcs(pDst, reinterpret_cast<const ansi **>(&pSrc), nSrcLength, &state);
#else
    return 0;
#endif // UNICODE
}
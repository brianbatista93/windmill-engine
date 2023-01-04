#include <errno.h>
#include <stdio.h>

#include "Containers/Array.hpp"
#include "Encoding/AnsiEncoder.hpp"
#include "Types.hpp"

extern i32 WindmillMain(i32 nArgC, tchar *pArgV[]);

i32 main(i32 nArgC, char *pArgV[])
{
    if (setenv("LC_NUMERIC", "en_US", 1) != 0)
    {
        fprintf(stderr, "Could not setenv(LC_NUMERIC): errno=%d (%s)", errno, strerror(errno));
        return errno;
    }

    CArray<tchar *, CAllocator<i32>> decodedArgV(nArgC);

    for (i32 i = 0; i < nArgC; ++i)
    {
        usize length = strlen(pArgV[i]);
        usize destLength = CAnsiEncoder::GetLength((const u8 *)pArgV[i], length);
        decodedArgV[i] = (tchar *)we_malloc(destLength * sizeof(tchar));
        CAnsiEncoder::Decode(decodedArgV[i], (u8 *)pArgV[i], length);
    }

    return WindmillMain(decodedArgV.GetSize(), decodedArgV.GetData());
}

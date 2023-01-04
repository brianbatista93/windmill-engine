#include "StringBuilder.hpp"
#include "WeString.hpp"

bool CFormatterArgument::TryFormat(tchar **pDest, const tchar *pFormat) const
{
    return mFormatter(pDest, pFormat);
}

void CStringBuilder::FormatInternal(const tchar *pFormat, usize nArgc, const CFormatterArgument *pArgs)
{
    const i32 nIndexLimit = 10000;
    const i32 nWidthLimit = 10000;

    const i32 nLength = i32(std::char_traits<tchar>::length(pFormat));

    if (nArgc == 0)
    {
        Append(pFormat, nLength);
        return;
    }

    i32 pos = 0;
    tchar chr = 0;

    while (true)
    {
        while (pos < nLength)
        {
            chr = pFormat[pos];

            pos++;

            if (chr == WT('}'))
            {
                we_assert(pos < nLength and pFormat[pos] == WT('}') && "Invalid pFormat string");
                pos++;
            }
            else if (chr == WT('{'))
            {
                if (pos < nLength && pFormat[pos] == WT('{'))
                {
                    pos++;
                }
                else
                {
                    pos--;
                    break;
                }
            }

            Append(chr);
        }

        if (pos == nLength)
        {
            break;
        }

        pos++;
        chr = pFormat[pos];
        we_assert(pos != nLength && isdigit(chr));

        size_t index = 0;
        do
        {
            index = index * 10 + chr - WT('0');
            pos++;
            we_assert(pos != nLength && "Invalid pFormat string");
            chr = pFormat[pos];
        } while (isdigit(chr) and index < nIndexLimit);

        we_assert(index < nArgc && "Invalid pFormat index");

        while (pos < nLength and isspace(chr = pFormat[pos]))
        {
            pos++;
        }

        bool leftPad = false;
        i32 width = 0;
        if (chr == WT(','))
        {
            pos++;

            while (pos < nLength and isspace(pFormat[pos]))
            {
                pos++;
            }

            we_assert(pos != nLength && "Invalid pFormat string");

            chr = pFormat[pos];
            if (chr == WT('-'))
            {
                leftPad = true;
                pos++;

                we_assert(pos != nLength && "Invalid pFormat string");
                chr = pFormat[pos];
            }

            we_assert(isdigit(chr) && "Invalid pFormat string");

            do
            {
                width = width * 10 + chr - WT('0');
                pos++;
                we_assert(pos != nLength && "Invalid pFormat string");
                chr = pFormat[pos];
            } while (isdigit(chr) and width < nWidthLimit);
        }

        while (pos < nLength and isspace(chr = pFormat[pos]))
        {
            pos++;
        }

        const CFormatterArgument argument = pArgs[index];
        CString itemFormatSubstring;

        if (chr == WT(':'))
        {
            pos++;
            const i32 startPos = pos;

            while (true)
            {
                we_assert(pos != nLength && "Invalid pFormat string");
                chr = pFormat[pos];

                if (chr == WT('}'))
                {
                    break;
                }

                we_assert(chr != WT('{') && "Invalid pFormat string");

                pos++;
            }

            if (pos > startPos)
            {
                itemFormatSubstring = CString(pFormat, usize(nLength)).Substring(startPos, pos - startPos);
            }
        }
        we_assert(chr == WT('}') && "Invalid pFormat string");

        pos++;
        CString formattedString;

        switch (argument.GetType())
        {
        case CFormatterArgument::kString: {
            formattedString = CString(argument.GetString());
        }
        break;
        case CFormatterArgument::kNumeric:
        case CFormatterArgument::kFormattable: {
            thread_local tchar buffer[128];
            memset(buffer, 0, 128 * sizeof(tchar));
            tchar *pBufferPtr = argument.GetType() == CFormatterArgument::kNumeric ? (std::end(buffer) - 1) : buffer;

            [[maybe_unused]] const bool formatted = !argument.TryFormat(&pBufferPtr, *itemFormatSubstring);
            we_assert(formatted && "Could not format argument");

            formattedString = CString((const tchar *)pBufferPtr);
        }
        break;
        default: {
            return;
        }
        break;
        }

        const i32 pad = width - formattedString.GetLength();
        if (!leftPad and (pad > 0))
        {
            Append(WT(' '), pad);
        }

        Append(formattedString);
        if (leftPad and (pad > 0))
        {
            Append(WT(' '), pad);
        }
    }
}

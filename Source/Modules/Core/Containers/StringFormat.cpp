#include "StringBuilder.hpp"
#include "WeString.hpp"

bool CFormatterArgument::TryFormat(tchar **pDest, const tchar *pFormat)
{
    return m_pFormatter(pDest, pFormat);
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
    tchar ch = 0;

    while (true)
    {
        while (pos < nLength)
        {
            ch = pFormat[pos];

            pos++;

            if (ch == WT('}'))
            {
                we_assert(pos < nLength and pFormat[pos] == WT('}') && "Invalid pFormat string");
                pos++;
            }
            else if (ch == WT('{'))
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

            Append(ch);
        }

        if (pos == nLength)
        {
            break;
        }

        pos++;
        we_assert(not(pos == nLength or !isdigit(ch = pFormat[pos])) && "Invalid pFormat string");
        size_t index = 0;
        do
        {
            index = index * 10 + ch - WT('0');
            pos++;
            we_assert(pos != nLength && "Invalid pFormat string");
            ch = pFormat[pos];
        } while (isdigit(ch) and index < nIndexLimit);

        we_assert(index < nArgc && "Invalid pFormat index");

        while (pos < nLength and isspace(ch = pFormat[pos]))
        {
            pos++;
        }

        bool leftPad = false;
        i32 width = 0;
        if (ch == WT(','))
        {
            pos++;

            while (pos < nLength and isspace(pFormat[pos]))
            {
                pos++;
            }

            we_assert(pos != nLength && "Invalid pFormat string");

            ch = pFormat[pos];
            if (ch == WT('-'))
            {
                leftPad = true;
                pos++;

                we_assert(pos != nLength && "Invalid pFormat string");
                ch = pFormat[pos];
            }

            we_assert(isdigit(ch) && "Invalid pFormat string");

            do
            {
                width = width * 10 + ch - WT('0');
                pos++;
                we_assert(pos != nLength && "Invalid pFormat string");
                ch = pFormat[pos];
            } while (isdigit(ch) and width < nWidthLimit);
        }

        while (pos < nLength and isspace(ch = pFormat[pos]))
        {
            pos++;
        }

        auto argument = pArgs[index];
        CString item_format_sub;

        if (ch == WT(':'))
        {
            pos++;
            i32 startPos = pos;

            while (true)
            {
                we_assert(pos != nLength && "Invalid pFormat string");
                ch = pFormat[pos];

                if (ch == WT('}'))
                {
                    break;
                }

                we_assert(ch != WT('{') && "Invalid pFormat string");

                pos++;
            }

            if (pos > startPos)
            {
                item_format_sub = CString(pFormat, usize(nLength)).Substring(startPos, pos - startPos);
            }
        }
        we_assert(ch == WT('}') && "Invalid pFormat string");

        pos++;
        CString s;

        switch (argument.GetType())
        {
        case CFormatterArgument::kString: {
            s = CString(argument.GetString());
        }
        break;
        case CFormatterArgument::kNumeric:
        case CFormatterArgument::kFormattable: {
            thread_local tchar buffer[128];
            memset(buffer, 0, 128 * sizeof(tchar));
            tchar *pBufferPtr = argument.GetType() == CFormatterArgument::kNumeric ? (std::end(buffer) - 1) : buffer;
            we_assert(argument.TryFormat(&pBufferPtr, *item_format_sub));

            s = CString((const tchar *)pBufferPtr);
        }
        break;
        default: {
            we_assert(false && "Invalid pFormat string");
        }
        break;
        }

        const i32 pad = width - s.GetLength();
        if (!leftPad and (pad > 0))
        {
            Append(WT(' '), pad);
        }

        Append(s);
        if (leftPad and (pad > 0))
        {
            Append(WT(' '), pad);
        }
    }
}

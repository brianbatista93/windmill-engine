#include "BinaryArchive.hpp"
#include "Containers/WeString.hpp"

void CBinaryArchive::SerializeString(CArchive &archive, CString &value)
{
    if (archive.IsReading())
    {
        i32 length;
        archive(length);

        if LIKELY (length)
        {
            auto &arr = value.GetArray();
            arr.Resize(length + 1);
            archive.Serialize(arr.GetData(), length * sizeof(tchar));
        }
    }
    else // Is writing
    {
        i32 length = value.GetLength();
        archive(length);
        archive.Serialize((void *)*value, length * sizeof(tchar));
    }
}

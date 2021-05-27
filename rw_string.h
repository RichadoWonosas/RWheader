#pragma once
#ifndef __RW_STRING__
#define __RW_STRING__ 100

#include "rw_vector.h"

namespace stdrw
{
    class String : public Vector<char>
    {
    public:
        explicit String(const char *char_string) : Vector<char>(char_string, string_length(char_string),
                                                                string_length(char_string)) {}

        static long string_length(const char *string_to_detect);

        String &operator<<(const char *append_string);
    };
}; // namespace stdrw

long stdrw::String::string_length(const char *string_to_detect)
{
    long length = 0;
    while (string_to_detect[length++] != '\0');

    return length;
}

stdrw::String &stdrw::String::operator<<(const char *append_string)
{
    Append(append_string, string_length(append_string));
    return *this;
}

#endif // __RW_STRING__
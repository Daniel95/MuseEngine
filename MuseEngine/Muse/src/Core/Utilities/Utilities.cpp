#include "MusePCH.h"

#include "Utilities.h"

#pragma warning( push )
#pragma warning( disable : 4706)
// String hashing function
// Taken from http://www.cse.yorku.ca/~oz/hash.html (public domain)
ullong Muse::StringHash(const std::string& a_String)
{
    ullong hash = 0;
    int c;
    const char* cstr = a_String.c_str();

    while (c = *cstr++)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

std::string Muse::PointerToString(void* a_Pointer)
{
    const void* address = static_cast<const void*>(a_Pointer);
    std::stringstream ss;
    ss << address;
    return ss.str();
}

bool Muse::Replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
    {
        return false;
    }
    str.replace(start_pos, from.length(), to);
    return true;
}

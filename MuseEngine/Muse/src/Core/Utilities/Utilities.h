#pragma once

#include "Core/Utilities/Defines.h"

#include <string>

namespace Muse
{
    /// String hash using sdbm algorithm
    ullong StringHash(const std::string& a_String);

    std::string PointerToString(const void* a_Pointer);

    bool Replace(std::string& a_Str, const std::string& a_From, const std::string& a_To);

}
#pragma once

#include "Core/Utilities/Defines.h"
#include <string>

namespace Muse
{
    /// String hash using sdbm algorithm
    ullong StringHash(const std::string& a_String);
    bool Replace(std::string& str, const std::string& from, const std::string& to);
}
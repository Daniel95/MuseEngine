#include "MusePCH.h"

#include "Utilities.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    #pragma warning( push )
    #pragma warning( disable : 4706)
    // String hashing function
    // Taken from http://www.cse.yorku.ca/~oz/hash.html (public domain)
    ullong StringHash(const std::string& a_String)
    {
        MUSE_PROFILE_FUNCTION();

        ullong hash = 0;
        int c;
        const char* cstr = a_String.c_str();

        while (c = *cstr++)
        {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }

    std::string PointerToString(const void* a_Pointer)
    {
        MUSE_PROFILE_FUNCTION();

        const void* address = static_cast<const void*>(a_Pointer);
        std::stringstream ss;
        ss << address;
        return ss.str();
    }

    bool Replace(std::string& a_Str, const std::string& a_From, const std::string& a_To)
    {
        MUSE_PROFILE_FUNCTION();

        size_t start_pos = a_Str.find(a_From);
        if (start_pos == std::string::npos)
        {
            return false;
        }
        a_Str.replace(start_pos, a_From.length(), a_To);
        return true;
    }

    const glm::vec3& Refract(glm::vec3 a_Vector, glm::vec3 a_Normal, float a_Eta)
    {
        float k = 1.0f - a_Eta * a_Eta * (1.0f - glm::dot(a_Normal, a_Vector) * glm::dot(a_Normal, a_Vector));

        if (k < 0.0f)
        {
            return glm::vec3();
        }
        else
        {
            return a_Eta * a_Vector - (a_Eta * glm::dot(a_Normal, a_Vector) + sqrt(k)) * a_Normal;
        }
    }
}
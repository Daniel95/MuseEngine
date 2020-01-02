#include "MusePCH.h"

#include "Resource.h"

namespace Muse
{
    void Resource::SetPathAndName(const std::string& a_Path)
    {
        /*
        m_Name = a_Path;
        Replace(m_Name, GAME_SCENE_PATH, "");
        Replace(m_Name, ".txt", "");
        */

        // Extract name from filepath
        // example: assets/shaders/FlatColor.glsl (between the last / and last .)
        auto lastSlash = a_Path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        const auto lastDot = a_Path.rfind('.');
        const auto count = lastDot == std::string::npos ? a_Path.size() - lastSlash : lastDot - lastSlash;
        m_Name = a_Path.substr(lastSlash, count);
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Resource>("Resource")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            );
    }
}

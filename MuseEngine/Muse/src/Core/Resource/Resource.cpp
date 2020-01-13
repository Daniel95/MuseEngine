#include "MusePCH.h"

#include "Resource.h"
#include "Core/Instrumentor.h"
#include "ResourceManager.h"

namespace Muse
{
    //Init's the path used for Resource ID, also updates the name.
    void Resource::InitPath(const std::string& a_Path)
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_Path == "", "Path already initialized, use UpdatePath!")

        m_Path = a_Path;
        m_Name = ExtractName(a_Path);
    }

    //Updates the path and change the resource ID accordingly, also updates the name.
    void Resource::UpdatePath(const std::string& a_Path)
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(m_Path != "", "Path is unitialized, use InitPath!");
        if (a_Path == m_Path) { return; }

        ResourceManager::UpdateResourcePath(m_Path, a_Path);

        m_Path = a_Path;
        m_Name = ExtractName(a_Path);
    }

    std::string Resource::ExtractName(const std::string& a_Path)
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

        return a_Path.substr(lastSlash, count);
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Resource>("Resource")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("m_Path", &Resource::GetPath, &Resource::InitPath);
    }
}

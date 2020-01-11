#include "MusePCH.h"
#include "FileBrowser.h"
#include "imgui.h"
#include "Core/Resource/Resource.h"

namespace Muse
{
    void FileBrowser::Render()
    {
        if (ImGui::Begin("FileBrowser"))
        {
            InspectResourceFiles("assets/");

            /*
            if (ImGui::CollapsingHeader("Loaded Resources"))
            {
                if (ImGui::SmallButton("Reload Resources"))
                {
                    LOG_ENGINE_INFO("Reload resources.");
                    for (std::pair<ullong, Resource*> pair : m_Resources)
                    {
                        pair.second->Reload();
                    }
                }

                for (std::pair<ullong, Resource*> pair : m_Resources)
                {
                    Resource* resource = pair.second;
                    ImGui::Text(resource->Path().c_str());
                }
            }
            */
        }
        ImGui::End();
    }

    void FileBrowser::InspectResourceFiles(const std::string& a_Path)
    {
        for (const auto& entry : std::filesystem::directory_iterator(a_Path))
        {
            ImGui::Text(entry.path().u8string().c_str());

            if (std::filesystem::is_directory(entry))
            {
                InspectResourceFiles(entry.path().u8string());
            }
        }
    }
}

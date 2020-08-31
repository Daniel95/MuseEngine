#include "SceneHierarchyPanel.h"

#include "Core/ECS/Component/TagComponent.h"

#include <imgui/imgui.h>

namespace Muse
{
    SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& a_Scene)
    {
        SetContext(a_Scene);
    }

    void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& a_Scene)
    {
        m_Context = a_Scene;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto a_EntityId)
        {
            Entity entity = { a_EntityId };

            auto& tagComponent = entity.GetComponent<TagComponent>();
            ImGui::Text("%s", tagComponent.m_Tag.c_str());
        });

        ImGui::End();
    }
}
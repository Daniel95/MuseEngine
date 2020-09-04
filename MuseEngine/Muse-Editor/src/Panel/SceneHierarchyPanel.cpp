#include "SceneHierarchyPanel.h"

#include "Core/ECS/Component/TagComponent.h"
#include "Core/Utilities/Defines.h"

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
            DrawEntityNode(entity);
        });

        ImGui::End();
    }

    const Entity* SceneHierarchyPanel::GetSelectedEntity() const
    {
        if (m_SelectionContext.GetEntityHandle() != entt::null)
        {
            return &m_SelectionContext;
        }
        else
        {
            return nullptr;
        }
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity a_Entity)
    {
        std::string& tag = a_Entity.GetComponent<TagComponent>().m_Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == a_Entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)a_Entity, flags, tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = a_Entity;
        }

        if (opened)
        {
            ImGui::TreePop();
        }
    }
}
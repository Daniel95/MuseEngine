#include "InspectorPanel.h"

#include "Core/ECS/Component/TagComponent.h"

#include <imgui/imgui.h>
#include "Core/ECS/Component/TransformComponent.h"
#include "entt.hpp"
#include "Core/ECS/Component/Render2DComponent.h"

namespace MM
{
    template <>
    void ComponentEditorWidget<Muse::TransformComponent>(entt::registry& reg, entt::registry::entity_type e)
    {
        auto& t = reg.get<Muse::TransformComponent>(e);
        ImGui::DragFloat("x", &t.m_LocalPosition.x, 0.1f);
        ImGui::DragFloat("y", &t.m_LocalPosition.y, 0.1f);
    }

    template <>
    void ComponentEditorWidget<Muse::Render2DComponent>(entt::registry& reg, entt::registry::entity_type e)
    {
        auto& t = reg.get<Muse::Render2DComponent>(e);
        ImGui::DragFloat("tilingFactor", &t.tilingFactor, 0.1f);
    }
}

namespace Muse
{
    InspectorPanel::InspectorPanel()
    {
        m_EntityEditor.registerComponent<TransformComponent>("Transform");
        m_EntityEditor.registerComponent<Render2DComponent>("Render2DComponent");


    }

    InspectorPanel::InspectorPanel(const std::shared_ptr<Scene>& a_Scene)
    {
    }

    void InspectorPanel::SetContext(const std::shared_ptr<Scene>& a_Scene)
    {
    }

    void InspectorPanel::OnImGuiRender(const Entity* a_Entity)
    {
        ImGui::Begin("Inspector");

        if (a_Entity != nullptr) 
        { 
            //Name
            std::string& tag = a_Entity->GetComponent<TagComponent>().m_Tag;
            ImGui::Text(tag.c_str());


            entt::registry& registry = SceneManager::GetActiveScene()->GetRegistry();

            //Transform
            TransformComponent& transformComponent = a_Entity->GetComponent<TransformComponent>();

            entt::entity entity = a_Entity->GetEntityHandle();

            m_EntityEditor.render(registry, entity);

            ImGui::Text(tag.c_str());

        }
        ImGui::End();
    }
}
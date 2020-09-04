#pragma once

#include "Core/Utilities/Defines.h"

#include "Core/ECS/Entity/Entity.h"
#include <memory>
#include "imgui_entt_entity_editor.h"

namespace Muse
{
    class InspectorPanel
    {
    public:
        InspectorPanel();
        InspectorPanel(const std::shared_ptr<Scene>& a_Scene);
        ~InspectorPanel() = default;

        void SetContext(const std::shared_ptr<Scene>& a_Scene);

        void OnImGuiRender(const Entity* a_Entity);
    private:
        std::shared_ptr<Scene> m_Context;
        Entity m_SelectionContext;
        MM::EntityEditor<entt::entity> m_EntityEditor;

    };
}
#pragma once

#include "Core/Scene/Scene.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/Utilities/Log.h"

#include <memory>

namespace Muse
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const std::shared_ptr<Scene>& a_Scene);
        ~SceneHierarchyPanel() = default;

        void SetContext(const std::shared_ptr<Scene>& a_Scene);

        void OnImGuiRender();

        const Entity* GetSelectedEntity() const;

    private:
        void DrawEntityNode(Entity a_Entity);

        std::shared_ptr<Scene> m_Context;
        Entity m_SelectionContext;
    };
}
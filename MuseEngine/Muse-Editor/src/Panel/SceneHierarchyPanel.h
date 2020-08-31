#pragma once

#include "Core/Utilities/Defines.h"
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

        void SetContext(const std::shared_ptr<Scene>& a_Scene);

        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity a_Entity);

        std::shared_ptr<Scene> m_Context;
        Entity m_SelectionContext;
    };
}
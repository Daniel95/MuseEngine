#pragma once

#include "Core/Resource/Resource.h"
#include "Core/ECS/Entity/Entity.h"

#include "entt.hpp"

#include <cereal/cereal.hpp>
#include <string>
#include <vector>

namespace Muse
{
    class Entity;
    class TransformComponent;

    class Scene : public Resource
    {
    public:
        Scene() = default;
        ~Scene() = default;

        entt::registry& GetRegistry() { return m_Registry; }

        Entity CreateEntity(const std::string& a_Name);
        Entity CreateEntity(const std::string& a_Name, TransformComponent& a_TransformComponent);
        void DestroyEntity(Entity a_Entity);

        static std::shared_ptr<Scene> Create() { return std::make_shared<Scene>(); }
        static std::shared_ptr<Scene> Load(const std::string& a_FilePath);

        void OnUpdate(float a_DeltaTime);
        void OnViewportResize(uint32_t a_Width, uint32_t a_Height);
        void SetName(const std::string& a_Name) { m_Name = a_Name; }
        void Save() { Save(m_Path); }
        void Save(const std::string& a_FilePath);
        void Clear();

    private:
        entt::registry m_Registry;
        std::vector<Entity> m_EntitiesToRemove;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class SceneHierarchyPanel;
    };
}

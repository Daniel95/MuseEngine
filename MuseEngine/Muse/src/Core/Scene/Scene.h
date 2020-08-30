#pragma once

#include "Core/Resource/Resource.h"
#include "Core/ECS/Entity/Entity.h"

#include "entt.hpp"
#include <cereal/cereal.hpp>

namespace Muse
{
    class Entity;
    struct TransformComponent;

    class Scene : public Resource
    {
    public:
        Scene();
        ~Scene() = default;

        entt::registry& GetRegistry() { return m_Registry; }

        Entity CreateEntity();
        Entity CreateEntity(TransformComponent& a_TransformComponent);

        static std::shared_ptr<Scene> Create() { return std::make_shared<Scene>(); }
        static std::shared_ptr<Scene> Load(const std::string& a_FilePath);

        void SetName(const std::string& a_Name) { m_Name = a_Name; }
        void Save() { Save(m_Path); }
        void Save(const std::string& a_FilePath);
        void Clear();

    private:
        entt::registry m_Registry;

    };
}

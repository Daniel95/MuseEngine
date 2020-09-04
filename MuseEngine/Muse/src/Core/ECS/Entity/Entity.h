#pragma once

#include "Core/Utilities/Defines.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"

#include "entt.hpp"

#include <vector>
#include <memory>
#include <string>

namespace Muse
{
    class TransformComponent;

    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle);
        Entity(const Entity& other) = default;

        static Entity Create(const std::string& a_Name);
        static Entity Create(const std::string& a_Name, TransformComponent& a_TransformComponent);

        void Destroy();
        entt::entity GetEntityHandle() const { return m_EntityHandle; }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) const
        {
            ASSERT_ENGINE(!HasComponent<T>(), "Entity already has component!");
            return SceneManager::GetActiveScene()->GetRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent() const
        {
            ASSERT_ENGINE(HasComponent<T>(), "Entity does not have component!");
            return SceneManager::GetActiveScene()->GetRegistry().get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()  const
        {
            return SceneManager::GetActiveScene()->GetRegistry().has<T>(m_EntityHandle);
        }

        /*
        //Maybe use get and check nullptr!
        template<typename T>
        bool HasComponent(T* a_Component)
        {
            std::shared_ptr<Scene> scene = SceneManager::GetActiveScene();

            bool hasComponent = scene->GetRegistry().has<T>(m_EntityHandle);
            if (hasComponent)
            {
                a_Component = &scene->GetRegistry().get<T>(m_EntityHandle);
                return true;
            }
            else
            {
                return false;
            }
        }
        */

        template<typename T>
        void RemoveComponent()
        {
            ASSERT_ENGINE(HasComponent<T>(), "Entity does not have component!");
            SceneManager::GetActiveScene()->GetRegistry().remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator ==(const Entity& a_Other) const { return m_EntityHandle == a_Other.m_EntityHandle; }
        bool operator !=(const Entity& a_Other) const { return !(*this == a_Other); }
    private:
        entt::entity m_EntityHandle{ entt::null };

    };
}
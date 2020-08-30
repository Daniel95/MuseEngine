#pragma once

#include "Core/Utilities/Defines.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"

#include "entt.hpp"

#include <vector>
#include <memory>

namespace Muse
{
	class Entity
	{
	public:
        Entity() = default;
        Entity(entt::entity handle);
        Entity(const Entity & other) = default;

        static Entity Create();

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            ASSERT_ENGINE(!HasComponent<T>(), "Entity already has component!");
            return SceneManager::GetActiveScene()->GetRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            ASSERT_ENGINE(HasComponent<T>(), "Entity does not have component!");
            return SceneManager::GetActiveScene()->GetRegistry().get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return SceneManager::GetActiveScene()->GetRegistry().has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            ASSERT_ENGINE(HasComponent<T>(), "Entity does not have component!");
            SceneManager::GetActiveScene()->GetRegistry().remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }

	private:
        entt::entity m_EntityHandle{ entt::null };
		
	};
}
#include "MusePCH.h"
#include "Entity.h"

namespace Muse
{
    Entity::Entity(entt::entity handle)
        : m_EntityHandle(handle)
    {
    }

	Entity Entity::Create(const std::string& a_Name)
	{
		Entity entity = SceneManager::GetActiveScene()->CreateEntity(a_Name);
		return entity;
	}

	Entity Entity::Create(const std::string& a_Name, TransformComponent& a_TransformComponent)
	{
        Entity entity = SceneManager::GetActiveScene()->CreateEntity(a_Name, a_TransformComponent);
        return entity;
	}
}

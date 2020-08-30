#include "MusePCH.h"
#include "Entity.h"

namespace Muse
{
    Entity::Entity(entt::entity handle)
        : m_EntityHandle(handle)
    {
    }

	Entity Entity::Create()
	{
		Entity entity = SceneManager::GetActiveScene()->CreateEntity();
		return entity;
	}

	Entity Entity::Create(TransformComponent& a_TransformComponent)
	{
        Entity entity = SceneManager::GetActiveScene()->CreateEntity(a_TransformComponent);
        return entity;
	}

	/*
	std::string Entity::GetName() const
	{
        if (Muse::EntityDebugger::HasEntityName(m_Id))
        {
            return Muse::EntityDebugger::GetEntityName(m_Id);
		}
		else
		{
			return std::to_string(m_Id);
		}
	}
	*/
}

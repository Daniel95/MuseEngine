#include "MusePCH.h"
#include "Entity.h"
#include "EntityTracker.h"
#include "Core/Utilities/VectorHelper.h"
#include "Core/ECS/Entity/EntityDebugger.h"

namespace Muse
{
    DestroyEvent Entity::s_DestroyEvent;
	DestroyAllEvent Entity::s_DestroyAllEvent;

	Entity::Entity(int a_Id)
		: m_Id(a_Id)
	{
	}

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

	Entity Entity::Create()
	{
		return Entity{ EntityTracker::Increment() };
	}

	Entity Entity::Create(const std::string& a_Name)
	{
		int id = EntityTracker::Increment();

		EntityDebugger::AddEntityName(id, a_Name);

        return Entity{ id };
	}

	void Entity::Destroy(Entity a_Entity)
	{
        EntityDebugger::RemoveEntityName(a_Entity);

		s_DestroyEvent.Dispatch(a_Entity);
	}

	void Entity::DestroyAll()
	{
        EntityDebugger::ClearEntityData();

        s_DestroyAllEvent.Dispatch();
	}
}

#include "MusePCH.h"
#include "Entity.h"
#include "EntityTracker.h"
#include "Core/Utilities/VectorHelper.h"
#include "Core/ECS/Entity/EntityDebugger.h"

namespace Muse
{
	DestroyEvent Entity::s_DestroyEvent;

	Entity::Entity(int a_Id)
		: m_Id(a_Id)
	{
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

	void Entity::Destroy(int a_Entity)
	{
        EntityDebugger::RemoveEntityName(a_Entity);

		s_DestroyEvent.Dispatch(a_Entity);
	}
}

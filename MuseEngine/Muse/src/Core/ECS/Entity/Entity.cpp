#include "MusePCH.h"
#include "Entity.h"
#include "EntityTracker.h"
#include "Core/Utilities/VectorHelper.h"

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

	void Entity::Destroy(int a_Entity)
	{
		s_DestroyEvent.Dispatch(a_Entity);
	}
}

#include "MusePCH.h"
#include "Entity.h"
#include "EntityTracker.h"
#include "Core/Utilities/VectorHelper.h"

namespace Muse
{
	Entity::Entity(int a_Id)
		: m_Id(a_Id)
	{
	}

	Entity Entity::Create()
	{
		return Entity{ EntityTracker::Increment() };
	}
}

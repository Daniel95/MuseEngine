#include "MusePCH.h"
#include "EntityTracker.h"

namespace Muse
{
	int EntityTracker::s_Index = 0;

	int EntityTracker::Increment()
	{
		s_Index++;
		return s_Index;
	}
}


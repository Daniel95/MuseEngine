#include "MusePCH.h"
#include "Layer.h"
#include "Core/Instrumentor.h"

namespace Muse
{
	Layer::Layer(const std::string& a_Name)
		: debugName(a_Name)
	{
		MUSE_PROFILE_FUNCTION();
	}
}

#pragma once

#include "Core/Event/Event.h"

namespace Muse 
{
	//Params: a_Width, a_Height.
	class WindowResizeEvent : public Event<int, int> { };

	class WindowCloseEvent : public Event<> { };

	class AppTickEvent : public Event<> { };

	class AppUpdateEvent : public Event<> { };

	class AppRenderEvent : public Event<> { };
}

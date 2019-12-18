#pragma once

#include "Core/Event.h"

namespace Muse 
{
	//Params: Width, Height.
	class WindowResizeEvent : public Event<int, int> { };

	class WindowCloseEvent : public Event<> { };

	class AppTickEvent : public Event<> {};

	class AppUpdateEvent : public Event<> {};

	class AppRenderEvent : public Event<> {};
}
#pragma once
#include "Core/Event.h"

namespace Muse 
{
	//Params: float x, y.
	class MouseMovedEvent : public Event<float, float> {};

	//Params: float xOffset, float yOffset.
	class MouseScrolledEvent : public Event<float, float> {};

	//Params: int button.
	class MouseButtonEvent : public Event<int> {};

	//Params: int button.
	class MouseButtonPressedEvent : public Event<int> {};

	//Params: int button.
	class MouseButtonReleasedEvent : public Event<int> {};
}
#pragma once

#include "Core/Event.h"

namespace Muse 
{
	//Params: Keycode, RepeatCount.
	class KeyPressedEvent : public Event<int, int> {};

	//Params: Keycode, RepeatCount.
	class KeyReleasedEvent : public Event<int> {};

	//Params: Keycode.
	class KeyTypedEvent : public Event<int> {};
}
#pragma once

#include "Core/Event/Event.h"

namespace Muse 
{
	//Params: a_Keycode, a_RepeatCount.
	class KeyPressedEvent : public Event<int, int> { };

	//Params: a_Keycode, a_RepeatCount.
	class KeyReleasedEvent : public Event<int> { };

	//Params: a_Keycode.
	class KeyTypedEvent : public Event<int> { };

}
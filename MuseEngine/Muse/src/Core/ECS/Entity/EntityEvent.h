#pragma once

#include "Core/Event/Event.h"

namespace Muse
{
    class Entity;

    class DestroyEvent : public Event<Entity> {};
}
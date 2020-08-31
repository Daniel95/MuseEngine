#pragma once

#include "Core/ECS/Entity/Entity.h"

namespace Muse
{
    class ScriptableEntity
    {
    public:
        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

    private:
        Entity m_Entity;
        friend class Scene;

    };
}
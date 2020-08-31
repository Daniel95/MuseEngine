#pragma once

#include "Core/ECS/Entity/Entity.h"

namespace Muse
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;

        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(float a_DeltaTime) {}

    private:
        Entity m_Entity;
        friend class Scene;

    };
}
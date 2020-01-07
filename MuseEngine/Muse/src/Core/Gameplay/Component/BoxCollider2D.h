#pragma once

#include "Core/Gameplay/Component/Component.h"

namespace Muse
{
    class GameObject;
    enum class CollisionType { None, Dynamic, Static };

    class BoxCollider2D : public Component
    {
    public:
        BoxCollider2D();
        virtual ~BoxCollider2D() = default;

        CollisionType GetCollisionType() const;
        void SetCollisionType(CollisionType a_CollisionType);

    private:
        float m_Scale = 1.f;
        CollisionType m_CollisionType;

        RTTR_ENABLE(Component)
        RTTR_REGISTRATION_FRIEND
    };
}
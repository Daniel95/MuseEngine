#pragma once

#include "Core/Gameplay/Component/Component.h"

namespace Muse
{
    class GameObject;
    enum class CollisionType { None, Dynamic, Static };

    class BoxCollider2D : public Component
    {
        RTTR_ENABLE(Component);

    public:
        BoxCollider2D();
        virtual ~BoxCollider2D() = default;

        CollisionType GetCollisionType();
        void SetCollisionType(CollisionType a_CollisionType);
        CollisionType RTTRGetCollisionType();
        void RTTRSetCollisionType(CollisionType a_CollisionType);

    private:
        float m_Scale = 1.f;
        CollisionType m_CollisionType;

    };
}
#pragma once

#include "Core/Gameplay/Component/Component.h"

namespace Muse
{
    class GameObject;
    enum class CollisionType { NONE, Dynamic, Static };

    class BoxCollider2D : public Component
    {
        RTTR_ENABLE(Component);

    public:
        BoxCollider2D();
        virtual ~BoxCollider2D();

        virtual void Init(GameObject* a_GameObject);
        virtual void Update(float a_DeltaTime) override;
        virtual void FixedUpdate(float a_TimeStep) override;

        CollisionType GetCollisionType();
        void SetCollisionType(CollisionType a_CollisionType);
        CollisionType RTTRGetCollisionType();
        void RTTRSetCollisionType(CollisionType a_CollisionType);

    private:
        float m_Scale = 1.f;
        CollisionType m_CollisionType;

    };
}
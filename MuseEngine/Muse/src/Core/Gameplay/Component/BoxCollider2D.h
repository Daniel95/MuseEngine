#pragma once
#include "Core/Gameplay/Component/Component.h"

namespace Muse
{
    class GameObject;
    enum class CollisionType { None, Dynamic, Static };

    class BoxCollider2D : public Component
    {
        RTTR_ENABLE(Component)
        RTTR_REGISTRATION_FRIEND

    public:
        BoxCollider2D();
        virtual ~BoxCollider2D() = default;

        CollisionType GetCollisionType() const;
        void SetCollisionType(CollisionType a_CollisionType);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Component", cereal::base_class<Component>(this)));
            ar(m_Scale, m_CollisionType);
        }
    private:
        float m_Scale = 1.f;
        CollisionType m_CollisionType;

    };
}

CEREAL_REGISTER_TYPE_WITH_NAME(Muse::BoxCollider2D, "BoxCollider2D")
CEREAL_REGISTER_POLYMORPHIC_RELATION(Muse::Component, Muse::BoxCollider2D)
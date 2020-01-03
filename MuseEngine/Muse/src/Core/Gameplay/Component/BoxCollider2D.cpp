#include "MusePCH.h"

#include "Core/Gameplay/Component/BoxCollider2D.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{
    BoxCollider2D::BoxCollider2D()
    {
        if (m_CollisionType != CollisionType::NONE)
        {
            SetCollisionType(m_CollisionType);
        }
    }

    BoxCollider2D::~BoxCollider2D()
    {
    }

    void BoxCollider2D::Update(float a_DeltaTime)
    {
        _ASSERT(m_CollisionType != CollisionType::NONE);
    }

    void BoxCollider2D::SetCollisionType(CollisionType a_CollisionType)
    {
        m_CollisionType = a_CollisionType;
    }

    CollisionType BoxCollider2D::RTTRGetCollisionType()
    {
        return m_CollisionType;
    }

    void BoxCollider2D::RTTRSetCollisionType(CollisionType a_CollisionType)
    {
        m_CollisionType = a_CollisionType;
    }

    CollisionType BoxCollider2D::GetCollisionType()
    {
        return m_CollisionType;
    }

    RTTR_REGISTRATION
    {
        rttr::registration::class_<Muse::BoxCollider2D>("BoxCollider2D")
            .constructor<>()
            (
                rttr::policy::ctor::as_raw_ptr
            )
            .property("CollisionType", &Muse::BoxCollider2D::RTTRGetCollisionType, &Muse::BoxCollider2D::RTTRSetCollisionType);

    rttr::registration::enumeration<Muse::CollisionType>("CollisionType")
        (
            rttr::value("NONE", Muse::CollisionType::NONE),
            rttr::value("Player", Muse::CollisionType::Static),
            rttr::value("Enemy", Muse::CollisionType::Dynamic)
        );

    }
}



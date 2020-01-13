#include "MusePCH.h"

#include "Core/Gameplay/Component/BoxCollider2D.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Instrumentor.h"
#include <cereal/archives/json.hpp>

namespace Muse
{
    BoxCollider2D::BoxCollider2D()
    {
        MUSE_PROFILE_FUNCTION();

        if (m_CollisionType != CollisionType::None)
        {
            SetCollisionType(m_CollisionType);
        }
    }

    void BoxCollider2D::SetCollisionType(CollisionType a_CollisionType)
    {
        m_CollisionType = a_CollisionType;
    }

    CollisionType BoxCollider2D::GetCollisionType() const
    {
        return m_CollisionType;
    }
}

RTTR_REGISTRATION
{
rttr::registration::enumeration<Muse::CollisionType>("CollisionType")
    (
        rttr::value("None", Muse::CollisionType::None),
        rttr::value("Player", Muse::CollisionType::Static),
        rttr::value("Enemy", Muse::CollisionType::Dynamic)
    );

    rttr::registration::class_<Muse::BoxCollider2D>("BoxCollider2D")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        )
        .property("m_CollisionType", &Muse::BoxCollider2D::m_CollisionType);
}
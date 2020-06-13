#include "MusePCH.h"

#include "Collision2DHelper.h"

#include "Core/ECS/Component/Collider2DComponent.h"

namespace Muse
{

    bool Collision2DHelper::AABBCheck(const BoundingBox& a_Collider1, const BoundingBox& a_Collider2)
    {
        return (a_Collider1.minX <= a_Collider2.maxX && a_Collider1.maxX >= a_Collider2.minX) &&
            (a_Collider1.minY <= a_Collider2.maxY && a_Collider1.maxY >= a_Collider2.minY);
    }
}
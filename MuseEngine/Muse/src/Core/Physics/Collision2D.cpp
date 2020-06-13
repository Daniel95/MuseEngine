#include "MusePCH.h"
#include "Collision2D.h"

#include "Core/ECS/Component/Collider2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/ComponentManager.h"

namespace Muse
{
    void Collision2D::GetEntityHits(const std::vector<int>& entities, std::vector<std::pair<int, int>>& hits)
    {
        std::unordered_map<int, Collider2DComponent>& collider2DComponents = ComponentManager<Collider2DComponent>::GetComponents();
        std::unordered_map<int, TransformComponent>& transformComponents = ComponentManager<TransformComponent>::GetComponents();

        for (auto thisEntity : entities)
        {
            for (auto otherEntity : entities)
            {
                if (thisEntity != otherEntity)
                {
                    BoundingBox thisBounds(transformComponents[thisEntity].localPosition, transformComponents[thisEntity].localScale);
                    BoundingBox otherBounds(transformComponents[otherEntity].localPosition, transformComponents[otherEntity].localScale);

                    if (AABBCheck(thisBounds, otherBounds))
                    {
                        hits.push_back({ thisEntity, otherEntity });
                    }
                }
            }

            collider2DComponents.erase(thisEntity);
        }
    }

    bool Collision2D::AABBCheck(const BoundingBox& a_Collider1, const BoundingBox& a_Collider2)
    {
        return (a_Collider1.minX <= a_Collider2.maxX && a_Collider1.maxX >= a_Collider2.minX) &&
            (a_Collider1.minY <= a_Collider2.maxY && a_Collider1.maxY >= a_Collider2.minY);
    }
}
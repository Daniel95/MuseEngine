#include "MusePCH.h"
#include "Collision2D.h"

#include "Core/ECS/Component/Collider2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/ComponentManager.h"

namespace Muse
{
    void Collision2D::GetEntityHits(const std::vector<int>& entityGroup1, const std::vector<int>& entityGroup2, std::vector<std::pair<int, int>>& hits)
    {
        const std::unordered_map<int, Collider2DComponent>& collider2DComponents = ComponentManager<Collider2DComponent>::GetComponents();
        const std::unordered_map<int, TransformComponent>& transformComponents = ComponentManager<TransformComponent>::GetComponents();

        for (auto entity1 : entityGroup1)
        {
            for (auto entity2 : entityGroup2)
            {
                BoundingBox thisBounds(transformComponents.at(entity1).localPosition, transformComponents.at(entity1).localScale);
                BoundingBox otherBounds(transformComponents.at(entity2).localPosition, transformComponents.at(entity2).localScale);

                if (AABBCheck(thisBounds, otherBounds))
                {
                    hits.push_back({ entity1, entity2 });
                }
            }
        }
    }

    bool Collision2D::AABBCheck(const BoundingBox& a_Collider1, const BoundingBox& a_Collider2)
    {
        return (a_Collider1.minX <= a_Collider2.maxX && a_Collider1.maxX >= a_Collider2.minX) &&
            (a_Collider1.minY <= a_Collider2.maxY && a_Collider1.maxY >= a_Collider2.minY);
    }
}
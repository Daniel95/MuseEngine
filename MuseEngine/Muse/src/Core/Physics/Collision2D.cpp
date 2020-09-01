#include "MusePCH.h"
#include "Collision2D.h"

#include "Core/ECS/Component/Collider2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/ComponentManager.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"

namespace Muse
{
    std::vector<std::pair<Entity, Entity>> Collision2D::m_CollidingEntities;
    bool Collision2D::m_CollisionsUpdated = nullptr;

    /*
    void Collision2D::GetEntityHits(const std::vector<int>& entityGroup1, const std::vector<int>& entityGroup2, std::vector<std::pair<int, int>>& hits)
    {
        std::shared_ptr<Scene> scene = SceneManager::GetActiveScene();

        const std::unordered_map<int, Collider2DComponent>& collider2DComponents = ComponentManager<Collider2DComponent>::GetComponents();
        const std::unordered_map<int, TransformComponent>& transformComponents = ComponentManager<TransformComponent>::GetComponents();

        for (auto entity1 : entityGroup1)
        {
            for (auto entity2 : entityGroup2)
            {
                if (entity1 != entity2)
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
    }
    */

    bool Collision2D::AABBCheck(const BoundingBox& a_Collider1, const BoundingBox& a_Collider2)
    {
        return (a_Collider1.minX <= a_Collider2.maxX && a_Collider1.maxX >= a_Collider2.minX) &&
            (a_Collider1.minY <= a_Collider2.maxY && a_Collider1.maxY >= a_Collider2.minY);
    }

    const std::vector<std::pair<Entity, Entity>>& Collision2D::GetCollidingEntities()
    {
        UpdateCollisions();

        return m_CollidingEntities;
    }

    void Collision2D::UpdateCollisions()
    {
        if (m_CollisionsUpdated) { return; }

        std::shared_ptr<Scene> scene = SceneManager::GetActiveScene();

        auto view = scene->GetRegistry().view<Collider2DComponent, TransformComponent>();

        m_CollidingEntities.clear();

        for (entt::entity thisEntityId : view)
        {
            for (entt::entity otherEntityId : view)
            {
                if (thisEntityId == otherEntityId) { continue; }

                //Collider2DComponent collider2DComponent1 = view.get<Collider2DComponent>(entityId1);
                TransformComponent transformComponent1 = view.get<TransformComponent>(thisEntityId);

                //Collider2DComponent collider2DComponent2 = view.get<Collider2DComponent>(entityId2);
                TransformComponent transformComponent2 = view.get<TransformComponent>(otherEntityId);

                BoundingBox thisBounds(transformComponent1.GetWorldPosition(), transformComponent1.GetWorldScale());
                BoundingBox otherBounds(transformComponent1.GetWorldPosition(), transformComponent1.GetWorldScale());

                if (AABBCheck(thisBounds, otherBounds))
                {
                    m_CollidingEntities.push_back({ thisEntityId, otherEntityId });
                }
            }
        }

        m_CollisionsUpdated = true;
    }
}
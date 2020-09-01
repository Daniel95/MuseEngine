#include "DestroyOutOfBoundsJob.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "BulletHell/Component/Components.h"
#include "Core/ECS/Entity/Entity.h"

void DestroyOutOfBoundsJob::OnUpdate()
{
    auto deleteOutOfBounds = [](
        Muse::Entity a_Entity,
        Muse::TransformComponent& a_TransformComponent, 
        DestroyOutOfBoundsComponent& a_DeleteOutOfBoundsComponent
        )
    {
        if (a_TransformComponent.GetWorldPosition().x > a_DeleteOutOfBoundsComponent.max.x ||
            a_TransformComponent.GetWorldPosition().x < a_DeleteOutOfBoundsComponent.min.x ||
            a_TransformComponent.GetWorldPosition().y > a_DeleteOutOfBoundsComponent.max.y ||
            a_TransformComponent.GetWorldPosition().y < a_DeleteOutOfBoundsComponent.min.y)
        {
            a_Entity.Destroy();
        }
    };

    Run<Muse::TransformComponent, DestroyOutOfBoundsComponent>(deleteOutOfBounds);
}


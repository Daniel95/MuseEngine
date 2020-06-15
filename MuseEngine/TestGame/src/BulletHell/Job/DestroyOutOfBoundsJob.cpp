#include "DestroyOutOfBoundsJob.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "BulletHell/Component/Components.h"

void DestroyOutOfBoundsJob::OnUpdate()
{
    auto deleteOutOfBounds = [](
        int a_Entity,
        Muse::TransformComponent& a_TransformComponent, 
        DestroyOutOfBoundsComponent& a_DeleteOutOfBoundsComponent
        )
    {
        if (a_TransformComponent.localPosition.x > a_DeleteOutOfBoundsComponent.max.x ||
            a_TransformComponent.localPosition.x < a_DeleteOutOfBoundsComponent.min.x ||
            a_TransformComponent.localPosition.y > a_DeleteOutOfBoundsComponent.max.y ||
            a_TransformComponent.localPosition.y < a_DeleteOutOfBoundsComponent.min.y)
        {
            Muse::Entity::Destroy(a_Entity);
        }
    };

    Run<Muse::TransformComponent, DestroyOutOfBoundsComponent>(deleteOutOfBounds);
}


#include "DeleteOutOfBoundsJob.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "BulletHell/Component/Components.h"

void DeleteOutOfBoundsJob::OnUpdate()
{
    auto deleteOutOfBounds = [](
        int a_Entity,
        Muse::TransformComponent& a_TransformComponent, 
        DeleteOutOfBoundsComponent& a_DeleteOutOfBoundsComponent
        )
    {
        if (a_TransformComponent.localPosition.y > 4 || a_TransformComponent.localPosition.y < -5)
        {
            Muse::Entity::Destroy(a_Entity);
        }
    };

    Run<Muse::TransformComponent, DeleteOutOfBoundsComponent>(deleteOutOfBounds);
}


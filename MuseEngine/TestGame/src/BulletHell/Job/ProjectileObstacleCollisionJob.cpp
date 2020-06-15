#include "ProjectileObstacleCollisionJob.h"

#include "BulletHell/Component/Components.h"

void ProjectileObstacleCollisionJob::OnUpdate()
{
    auto projectileCollision = [](
        int projectileEntity,
        ProjectileComponent& a_ProjectileComponent,
        ObstacleComponent& a_ObstacleComponent
        )
    {
        Muse::Entity::Destroy(projectileEntity);
    };

    auto obstacleCollision = [](
        int obstacleEntitiy,
        ProjectileComponent& a_ProjectileComponent,
        ObstacleComponent& a_ObstacleComponent
        )
    {
        Muse::Entity::Destroy(obstacleEntitiy);
    };

    RunCollision<ProjectileComponent, ObstacleComponent>(projectileCollision, obstacleCollision);
}

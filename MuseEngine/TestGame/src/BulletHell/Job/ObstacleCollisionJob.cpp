#include "MusePCH.h"

#include "ObstacleCollisionJob.h"
#include "BulletHell/Component/Components.h"

void ObstacleCollisionJob::OnUpdate()
{
    auto playerCollision = [](
        int playerEntitiy,
        PlayerComponent& a_PlayerComponent,
        int obstacleEntitiy,
        ObstacleComponent& a_ObstacleComponent
        )
    {
        //Player
        a_PlayerComponent.health -= a_ObstacleComponent.damage;

        //Obstacle
        Muse::Entity::Destroy(obstacleEntitiy);
    };

    RunCollision<PlayerComponent, ObstacleComponent>(playerCollision);

    auto obstacleOnObstacleCollision = [](
        int obstacleEntitiy1,
        ObstacleComponent& a_ObstacleComponent1,
        int obstacleEntitiy2,
        ObstacleComponent& a_ObstacleComponent2
        )
    {
        Muse::Entity::Destroy(obstacleEntitiy1);
    };

    RunCollision<ObstacleComponent, ObstacleComponent>(obstacleOnObstacleCollision);
}

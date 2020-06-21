#include "MusePCH.h"

#include "Core/ECS/Entity/Entity.h"

#include "ObstacleCollisionJob.h"
#include "BulletHell/Component/Components.h"
#include "BulletHell.h"

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

        if (a_PlayerComponent.health < 0)
        {
            Muse::Entity::DestroyAll();
            BulletHell::CreatePlayer();
        }
   
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

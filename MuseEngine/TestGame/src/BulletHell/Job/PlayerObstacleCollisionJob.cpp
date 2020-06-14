#include "MusePCH.h"

#include "PlayerObstacleCollisionJob.h"
#include "BulletHell/Component/Components.h"

void PlayerObstacleCollisionJob::OnUpdate()
{
    auto playerCollision = [](
        int playerEntitiy,
        PlayerComponent& a_PlayerComponent,
        ObstacleComponent& a_ObstacleComponent
        )
    {
        a_PlayerComponent.health -= a_ObstacleComponent.damage;

        LOG_INFO("Collision %d", a_PlayerComponent.health);
    };

    auto obstacleCollision = [](
        int obstacleEntitiy,
        PlayerComponent& a_PlayerComponent,
        ObstacleComponent& a_ProjectileComponent
        )
    {
        Muse::Entity::Destroy(obstacleEntitiy);

        LOG_INFO("a_ProjectileComponent");
    };

    RunCollision<PlayerComponent, ObstacleComponent>(playerCollision, obstacleCollision);
}

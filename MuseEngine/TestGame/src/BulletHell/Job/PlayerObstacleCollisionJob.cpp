#include "MusePCH.h"

#include "PlayerObstacleCollisionJob.h"
#include "BulletHell/Component/Components.h"

void PlayerObstacleCollisionJob::OnUpdate()
{
    auto moveSimple = [](
        int entitiy,
        PlayerComponent& a_PlayerComponent,
        ObstacleComponent& a_ObstacleComponent
        )
    {
        LOG_INFO("Collision");
    };

    RunCollision<PlayerComponent, ObstacleComponent>(moveSimple);
}

#include "MusePCH.h"

#include "Core/ECS/Entity/Entity.h"
#include "Core/Utilities/Utilities.h"

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
        Muse::Entity::Destroy(obstacleEntitiy2);
    };

    RunCollision<ObstacleComponent, ObstacleComponent>(obstacleOnObstacleCollision);

    auto projectileOnEnemyCollision = [](
        int a_EnemyEntitiy,
        EnemyComponent& a_EnemyComponent,
        Muse::TransformComponent& a_TransformComponent1,
        int a_ProjectileEntitiy,
        ProjectileComponent& a_ProjectileComponent,
        Muse::TransformComponent& a_TransformComponent2
        )
    {
        Muse::Entity::Destroy(a_EnemyEntitiy);
        Muse::Entity::Destroy(a_ProjectileEntitiy);

        SpawnBullets(a_TransformComponent1.localPosition);
    };

    RunCollision<EnemyComponent, ProjectileComponent>(projectileOnEnemyCollision);
}

void ObstacleCollisionJob::SpawnBullets(glm::vec2 a_Position, float a_ProjectileSpeed)
{
    float offset = 0.7f;

    //Up
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x, a_Position.y + offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Up left
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x - offset, a_Position.y + offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(45.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //left
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x - offset, a_Position.y, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(90.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Down left
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x - offset, a_Position.y - offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(135.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Down
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x, a_Position.y - offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(180.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Down right
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x + offset, a_Position.y - offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(225.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Right
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x + offset, a_Position.y, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(270.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Up Right
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x + offset, a_Position.y + offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(315.0f)),
        };

        BulletHell::CreateProjectile(projectileTransformComponent, a_ProjectileSpeed);
    }
}

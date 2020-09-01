#include "MusePCH.h"

#include "Core/ECS/Entity/Entity.h"
#include "Core/Utilities/Utilities.h"
#include "Core/ECS/Component/TransformComponent.h"

#include "ObstacleCollisionJob.h"
#include "BulletHell/Component/Components.h"
#include "BulletHell.h"
#include "Core/Scene/SceneManager.h"

void ObstacleCollisionJob::OnUpdate()
{
    auto playerCollision = [](
        Muse::Entity playerEntitiy,
        Muse::Entity obstacleEntitiy
        )
    {
        if (playerEntitiy.HasComponent<PlayerComponent>() && obstacleEntitiy.HasComponent<ObstacleComponent>())
        {
            PlayerComponent& playerComponent = playerEntitiy.GetComponent<PlayerComponent>();
            ObstacleComponent& obstacleComponent = obstacleEntitiy.GetComponent<ObstacleComponent>();

            //Player
            playerComponent.health -= obstacleComponent.damage;

            if (playerComponent.health < 0)
            {
                Muse::SceneManager::GetActiveScene()->Clear();
                BulletHell::CreatePlayer();
            }
   
            //Obstacle
            obstacleEntitiy.Destroy();
        }
    };

    RunCollision(playerCollision);

    auto obstacleOnObstacleCollision = [](
        Muse::Entity obstacleEntitiy1,
        Muse::Entity obstacleEntitiy2
        )
    {
        if (obstacleEntitiy1.HasComponent<ObstacleComponent>() && obstacleEntitiy2.HasComponent<ObstacleComponent>())
        {
            obstacleEntitiy1.Destroy();
            obstacleEntitiy2.Destroy();
        }
    };

    RunCollision(obstacleOnObstacleCollision);

    auto projectileOnEnemyCollision = [](
        Muse::Entity a_EnemyEntitiy,
        Muse::Entity a_ProjectileEntitiy
        )
    {
        if (a_EnemyEntitiy.HasComponent<EnemyComponent>() && a_ProjectileEntitiy.HasComponent<ProjectileComponent>())
        {
            a_EnemyEntitiy.Destroy();
            a_ProjectileEntitiy.Destroy();

            SpawnBullets(a_EnemyEntitiy.GetComponent<Muse::TransformComponent>().GetWorldPosition());
        }
    };
}

void ObstacleCollisionJob::SpawnBullets(glm::vec2 a_Position, float a_ProjectileSpeed)
{
    float offset = 0.3f;

    //Up
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x, a_Position.y + offset, 0),
            glm::vec3(0.25f),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Up left
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x - offset, a_Position.y + offset, 0),
            glm::vec3(0.25f),
            glm::vec3(0, 0, glm::radians(45.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //left
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x - offset, a_Position.y, 0),
            glm::vec3(0.25f),
            glm::vec3(0, 0, glm::radians(90.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Down left
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x - offset, a_Position.y - offset, 0),
            glm::vec3(0.25f),
            glm::vec3(0, 0, glm::radians(135.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Down
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x, a_Position.y - offset, 0),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(0, 0, glm::radians(180.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Down right
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x + offset, a_Position.y - offset, 0),
            glm::vec3(0.25f),
            glm::vec3(0, 0, glm::radians(225.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Right
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x + offset, a_Position.y, 0),
            glm::vec3(0.25f),
            glm::vec3(0, 0, glm::radians(270.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }

    //Up Right
    {
        Muse::TransformComponent projectileTransformComponent
        {
            glm::vec3(a_Position.x + offset, a_Position.y + offset, 0),
            glm::vec3(0.25f),
            glm::vec3(0, 0, glm::radians(315.0f)),
        };

        BulletHell::CreateProjectileRed(projectileTransformComponent, a_ProjectileSpeed);
    }
}

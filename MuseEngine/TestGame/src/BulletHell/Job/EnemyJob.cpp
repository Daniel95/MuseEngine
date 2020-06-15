#include "EnemyJob.h"

#include "BulletHell/Component/Components.h"
#include "Core/Input/Input.h"
#include "Muse.h"

void EnemyJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    std::shared_ptr<Muse::Texture> m_ProjectileTexture = Muse::ResourceManager::Get<Muse::Texture>("assets/textures/Checkerboard.png");

    auto playerCollision = [deltaTime, m_ProjectileTexture](
        int playerEntitiy,
        Muse::TransformComponent& a_TransformComponent,
        EnemyComponent& a_PlayerComponent
        )
    {
        a_PlayerComponent.fireTimer += deltaTime;

        if (a_PlayerComponent.fireTimer > a_PlayerComponent.fireCD)
        {
            a_PlayerComponent.fireTimer = 0;

            auto projectileEntity = Muse::Entity::Create();

            Muse::Render2DComponent render2DComponent
            {
                m_ProjectileTexture,
            };

            Muse::TransformComponent transformComponent
            {
                a_TransformComponent.localPosition + Muse::TransformHelper::GetUp(a_TransformComponent),
                glm::vec3(1, 1, 0),
                a_TransformComponent.localRotation,
            };

            Muse::ComponentManager<Muse::Render2DComponent>::Add(projectileEntity, render2DComponent);
            Muse::ComponentManager<Muse::TransformComponent>::Add(projectileEntity, transformComponent);
            Muse::ComponentManager<Muse::Collider2DComponent>::Add(projectileEntity, { });

            Muse::ComponentManager<ObstacleComponent>::Add(projectileEntity, { });
            Muse::ComponentManager<ProjectileComponent>::Add(projectileEntity, { 12 });
            Muse::ComponentManager<DestroyOutOfBoundsComponent>::Add(projectileEntity, { });
        }
    };

    Run<Muse::TransformComponent, EnemyComponent>(playerCollision);
}

#include "EnemyJob.h"

#include "BulletHell/Component/Components.h"
#include "BulletHell.h"
#include "Core/Input/Input.h"
#include "Muse.h"

void EnemyJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    auto playerCollision = [deltaTime](
        int playerEntitiy,
        Muse::TransformComponent& a_TransformComponent,
        EnemyComponent& a_PlayerComponent
        )
    {
        a_PlayerComponent.fireTimer += deltaTime;

        if (a_PlayerComponent.fireTimer > a_PlayerComponent.fireCD)
        {
            a_PlayerComponent.fireTimer = 0;

            Muse::TransformComponent projectileTransformComponent
            {
                a_TransformComponent.GetWorldPosition() + a_TransformComponent.GetUp(),
                glm::vec3(0.25f),
                a_TransformComponent.GetWorldRotation(),
            };

            BulletHell::CreateProjectileRed(projectileTransformComponent, 7);
        }
    };

    Run<Muse::TransformComponent, EnemyComponent>(playerCollision);
}

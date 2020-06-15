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
                a_TransformComponent.localPosition + Muse::TransformHelper::GetUp(a_TransformComponent),
                glm::vec3(1, 1, 0),
                a_TransformComponent.localRotation,
            };

            BulletHell::CreateProjectile(projectileTransformComponent, 1.0f);
        }
    };

    Run<Muse::TransformComponent, EnemyComponent>(playerCollision);
}

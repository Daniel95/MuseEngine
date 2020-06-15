#include "PlayerJob.h"

#include "Core/Input/Input.h"
#include <Muse.h>
#include "BulletHell/Component/Components.h"
#include "BulletHell.h"

void PlayerJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    glm::vec2 inputDirection = glm::vec2(0);

    if (Muse::Input::GetKeyDown(MUSE_KEY_A))
    {
        inputDirection.x = -1;
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_D))
    {
        inputDirection.x = 1;
    }

    if (Muse::Input::GetKeyDown(MUSE_KEY_S))
    {
        inputDirection.y = -1;
    }
    else if (Muse::Input::GetKeyDown(MUSE_KEY_W))
    {
        inputDirection.y = 1;
    }

    auto moveSimple = [inputDirection, deltaTime](
        int entity,
        Muse::TransformComponent& a_TransformComponent,
        PlayerComponent& a_PlayerComponent
        )
    {
        if (inputDirection != glm::vec2(0))
        {
            const glm::vec2 direction = glm::normalize(inputDirection);
            const glm::vec2 movement = inputDirection * a_PlayerComponent.speed * deltaTime;

            Muse::TransformHelper::TranslateLocal(a_TransformComponent, glm::vec3(movement.x, movement.y, 0));
        }

        a_PlayerComponent.fireTimer += deltaTime;

        if (Muse::Input::GetKeyDown(MUSE_KEY_SPACE) && 
            a_PlayerComponent.fireTimer > a_PlayerComponent.fireCD)
        {
            a_PlayerComponent.fireTimer = 0;

            Muse::TransformComponent projectileTransformComponent
            {
                glm::vec3(a_TransformComponent.localPosition.x, a_TransformComponent.localPosition.y + 1.1f, 0),
                glm::vec3(1, 1, 0)
            };

            BulletHell::CreateProjectile(projectileTransformComponent, 1.0f);
        }
    };

    Run<Muse::TransformComponent, PlayerComponent>(moveSimple);
}

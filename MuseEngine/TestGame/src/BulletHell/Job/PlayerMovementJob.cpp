#include "PlayerMovementJob.h"
#include "Core/ECS/Component/ComponentManager.h"

#include "Core/Input/Input.h"
#include <Muse.h>
#include "../Component/Components.h"

void PlayerMovementJob::OnUpdate()
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
        LOG_INFO(inputDirection.x);
        LOG_INFO(inputDirection.y);
        if (inputDirection != glm::vec2(0))
        {
            const glm::vec2 direction = glm::normalize(inputDirection);
            const glm::vec2 movement = inputDirection * a_PlayerComponent.speed * deltaTime;

            Muse::TransformHelper::TranslateLocal(a_TransformComponent, glm::vec3(movement.x, movement.y, 0));
        }
    };

    Run<Muse::TransformComponent, PlayerComponent>(moveSimple);
}

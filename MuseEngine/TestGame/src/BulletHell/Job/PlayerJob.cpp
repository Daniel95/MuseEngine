#include "PlayerJob.h"
#include "Core/ECS/Component/ComponentManager.h"

#include "Core/Input/Input.h"
#include <Muse.h>
#include "../Component/Components.h"

void PlayerJob::OnUpdate()
{
    auto move = [](
        const std::vector<int>& a_Entities,
        std::unordered_map<int, Muse::TransformComponent>& a_TransformComponents,
        std::unordered_map<int, PlayerComponent>& a_PlayerComponent
        )
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

        if (inputDirection != glm::vec2(0))
        {
            for (int entity : a_Entities)
            {
                auto transformIt = a_TransformComponents.find(entity);
                const auto moveIt = a_PlayerComponent.find(entity);

                const glm::vec2 movement = inputDirection * moveIt->second.speed * deltaTime;

                Muse::TransformHelper::TranslateLocal(transformIt->second, glm::vec3(movement.x, movement.y, 0));
            }
        }
    };

    Run<Muse::TransformComponent, PlayerComponent>(move);
}

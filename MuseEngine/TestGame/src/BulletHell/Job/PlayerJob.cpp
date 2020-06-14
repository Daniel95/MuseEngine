#include "PlayerJob.h"

#include "Core/Input/Input.h"
#include <Muse.h>
#include "BulletHell/Component/Components.h"

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

    std::shared_ptr<Muse::Texture> m_ProjectileTexture = Muse::ResourceManager::Get<Muse::Texture>("assets/textures/Checkerboard.png");

    auto moveSimple = [inputDirection, deltaTime, m_ProjectileTexture](
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

        //if (Muse::Input::GetKeyDown(MUSE_KEY_SPACE))
        //{
            auto projectileEntity = Muse::Entity::Create();

            Muse::Render2DComponent render2DComponent
            {
                m_ProjectileTexture,
            };

            Muse::TransformComponent transformComponent
            {
                glm::vec3(a_TransformComponent.localPosition.x, a_TransformComponent.localPosition.y + 1.1f, 0),
                glm::vec3(1, 1, 0)
            };

            Muse::ComponentManager<Muse::Render2DComponent>::Add(projectileEntity, render2DComponent);
            Muse::ComponentManager<Muse::TransformComponent>::Add(projectileEntity, transformComponent);
            Muse::ComponentManager<Muse::Collider2DComponent>::Add(projectileEntity, { });

            Muse::ComponentManager<ObstacleComponent>::Add(projectileEntity, { });
            Muse::ComponentManager<MoveForwardComponent>::Add(projectileEntity, { });
            Muse::ComponentManager<DeleteOutOfBoundsComponent>::Add(projectileEntity, { });
        //}
    };

    Run<Muse::TransformComponent, PlayerComponent>(moveSimple);
}

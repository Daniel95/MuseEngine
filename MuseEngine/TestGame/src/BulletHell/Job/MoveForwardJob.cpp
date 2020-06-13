#include "MoveForwardJob.h"

#include "Core/Application.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "BulletHell/Component/Components.h"

void MoveForwardJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    auto moveForward = [deltaTime](
        int entity,
        MoveForwardComponent& a_MoveforwardComponent,
        Muse::TransformComponent& a_TransformComponent
        )
    {
        float movement = a_MoveforwardComponent.speed * deltaTime;
        Muse::TransformHelper::TranslateLocal(a_TransformComponent, glm::vec3(0, movement, 0));
    };

    Run<MoveForwardComponent, Muse::TransformComponent>(moveForward);
}

#include "ProjectileJob.h"

#include "Core/Application.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "BulletHell/Component/Components.h"

void ProjectileJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    auto moveForward = [deltaTime](
        int entity,
        ProjectileComponent& a_MoveforwardComponent,
        Muse::TransformComponent& a_TransformComponent
        )
    {
        float movement = a_MoveforwardComponent.speed * deltaTime;
        Muse::TransformHelper::TranslateLocal(a_TransformComponent, Muse::TransformHelper::GetUp(a_TransformComponent) * movement);
    };

    Run<ProjectileComponent, Muse::TransformComponent>(moveForward);
}

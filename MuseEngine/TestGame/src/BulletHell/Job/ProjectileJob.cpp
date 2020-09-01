#include "ProjectileJob.h"

#include "Core/Application.h"
#include "Core/ECS/Component/TransformComponent.h"
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
        a_TransformComponent.TranslateLocal(a_TransformComponent.GetUp() * movement);
    };

    Run<ProjectileComponent, Muse::TransformComponent>(moveForward);
}

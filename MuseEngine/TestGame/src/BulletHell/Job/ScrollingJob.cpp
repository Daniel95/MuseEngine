#include "ScrollingJob.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "BulletHell/Component/Components.h"
#include "Core/Application.h"

void ScrollingJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    auto scrollJob = [deltaTime](
        int a_Entity,
        Muse::TransformComponent& a_TransformComponent,
        ScrollingComponent& a_ScrollingComponent
        )
    {
        a_TransformComponent.TranslateLocal(glm::vec3(0, -a_ScrollingComponent.speed * deltaTime, 0));
    };

    Run<Muse::TransformComponent, ScrollingComponent>(scrollJob);
}

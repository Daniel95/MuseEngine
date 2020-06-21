#include "ScrollingJob.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/TransformHelper.h"

#include "BulletHell/Component/Components.h"

void ScrollingJob::OnUpdate()
{
    float deltaTime = Muse::Application::Get().GetDeltaTime();

    auto scrollJob = [deltaTime](
        int a_Entity,
        Muse::TransformComponent& a_TransformComponent,
        ScrollingComponent& a_ScrollingComponent
        )
    {
        glm::vec3 newPosition = a_TransformComponent.localPosition + glm::vec3(0, -a_ScrollingComponent.speed * deltaTime , 0);

        Muse::TransformHelper::SetLocalPosition(a_TransformComponent, newPosition);
    };

    Run<Muse::TransformComponent, ScrollingComponent>(scrollJob);
}

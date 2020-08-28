#include "CameraJob.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "CameraComponent.h"

namespace Muse
{
    void CameraJob::OnUpdate()
    {
        auto updateCamera = [](
            int entity,
            TransformComponent& a_TransformComponent,
            CameraComponent& a_CameraComponent
            )
        {
            /*
            if (a_CameraComponent.m_IsDirty)
            {
                a_CameraComponent.m_ViewMatrix = glm::inverse(TransformHelper::GetWorldModelMatrix(a_TransformComponent));
                a_CameraComponent.m_ViewProjectionMatrix = a_CameraComponent.m_ProjectionMatrix * a_CameraComponent.m_ViewMatrix;
            }

            CameraHelper::GetProjecttionViewMatrix(a_CameraComponent, a_TransformComponent);
            */
        };

        Run<TransformComponent, CameraComponent>(updateCamera);
    }
}
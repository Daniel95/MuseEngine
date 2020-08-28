#include "OrthographicEditorCameraControllerJob.h"
#include "OrthographicEditorCameraControllerComponent.h"
#include "CameraComponent.h"
#include "CameraHelper.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"
#include "Core/ECS/Component/TransformHelper.h"

namespace Muse
{
    void Muse::OrthographicEditorCameraControllerJob::OnUpdate()
    {
        float deltaTime = Muse::Application::Get().GetDeltaTime();

        auto updateCamera = [deltaTime](
            int entity,
            TransformComponent& transformComponent,
            CameraComponent& cameraComponent,
            OrthographicEditorCameraControllerComponent& orthographicCameraControllerComponent
            )
        {
            const float moveSpeed = orthographicCameraControllerComponent.speed * deltaTime * cameraComponent.GetZoomLevel();

            if (Input::GetKeyDown(MUSE_KEY_A))
            {
                TransformHelper::TranslateLocal(transformComponent, glm::vec2(-moveSpeed, 0.0f));
            }
            else if (Input::GetKeyDown(MUSE_KEY_D))
            {
                TransformHelper::TranslateLocal(transformComponent, glm::vec2(moveSpeed, 0.0f));
            }

            if (Input::GetKeyDown(MUSE_KEY_S))
            {
                TransformHelper::TranslateLocal(transformComponent, glm::vec2(0.0f, -moveSpeed));
            }
            else if (Input::GetKeyDown(MUSE_KEY_W))
            {
                TransformHelper::TranslateLocal(transformComponent, glm::vec2(0.0f, moveSpeed));
            }
        };

        Run<TransformComponent, CameraComponent, OrthographicEditorCameraControllerComponent>(updateCamera);
    }
}
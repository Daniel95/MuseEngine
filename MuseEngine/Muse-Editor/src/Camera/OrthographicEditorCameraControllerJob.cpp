#include "OrthographicEditorCameraControllerJob.h"
#include "OrthographicEditorCameraControllerComponent.h"
#include "CameraComponent.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"
#include "Core/ECS/Component/TransformHelper.h"
#include "Core/Application.h"

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
            float zoomLevel = cameraComponent.GetZoomLevel() - (Input::GetMouseScrollDelta() * orthographicCameraControllerComponent.m_ScrollSpeed);
            zoomLevel = std::min(std::max(zoomLevel, orthographicCameraControllerComponent.m_MinZoomLevel), orthographicCameraControllerComponent.m_MaxZoomLevel);
            cameraComponent.SetZoomLevel(zoomLevel);

            if (Input::GetMouseButtonDown(1)) 
            { 
                const float moveSpeed = orthographicCameraControllerComponent.m_MoveSpeed * deltaTime * cameraComponent.GetZoomLevel();

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
            }
        };

        Run<TransformComponent, CameraComponent, OrthographicEditorCameraControllerComponent>(updateCamera);
    }
}
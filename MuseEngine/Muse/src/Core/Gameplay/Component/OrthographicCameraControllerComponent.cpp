#include "MusePCH.h"
#include "OrthographicCameraControllerComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "TransformComponent.h"

namespace Muse
{
    void OrthographicCameraControllerComponent::Update(float a_DeltaTime)
    {
        CameraComponent::Update(a_DeltaTime);

        if (Input::IsKeyPressed(MUSE_KEY_LEFT))
        {
            GetTransform()->Move(glm::vec2(-m_CameraMoveSpeed * a_DeltaTime, 0.0f));
        }
        else if (Input::IsKeyPressed(MUSE_KEY_RIGHT))
        {
            GetTransform()->Move(glm::vec2(m_CameraMoveSpeed * a_DeltaTime, 0.0f));
        }

        if (Input::IsKeyPressed(MUSE_KEY_DOWN))
        {
            GetTransform()->Move(glm::vec2(0.0f, -m_CameraMoveSpeed * a_DeltaTime));
        }
        else if (Input::IsKeyPressed(MUSE_KEY_UP))
        {
            GetTransform()->Move(glm::vec2(0.0f, m_CameraMoveSpeed * a_DeltaTime));
        }
    }
}

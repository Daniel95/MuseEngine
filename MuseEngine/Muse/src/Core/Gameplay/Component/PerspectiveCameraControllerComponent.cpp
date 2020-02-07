#include "MusePCH.h"
#include "PerspectiveCameraControllerComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "TransformComponent.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "CameraComponent.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Utilities/Defines.h"

namespace Muse
{
    PerspectiveCameraControllerComponent::PerspectiveCameraControllerComponent()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().GetWindow().MouseScrolledEvent.Subscribe(SUB_FN(PerspectiveCameraControllerComponent::OnMouseScrolledEvent, std::placeholders::_1, std::placeholders::_2));
        Application::Get().GetWindow().WindowResizeEvent.Subscribe(SUB_FN(PerspectiveCameraControllerComponent::OnWindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
    }

    PerspectiveCameraControllerComponent::~PerspectiveCameraControllerComponent()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().GetWindow().MouseScrolledEvent.Unsubscribe(this);
        Application::Get().GetWindow().WindowResizeEvent.Unsubscribe(this);
    }

    void PerspectiveCameraControllerComponent::OnInit()
    {
        MUSE_PROFILE_FUNCTION();

        m_CameraComponent = GetGameObject()->GetComponent<CameraComponent>();

        ASSERT_ENGINE(m_CameraComponent != nullptr, "CameraComponent on PerspectiveCameraControllerComponent GameObject is missing!");

        const float width = static_cast<float>(Application::Get().GetWindow().GetWidth());
        const float height = static_cast<float>(Application::Get().GetWindow().GetHeight());
        const float aspectRatio = width / height;

        m_CameraComponent->SetProjection(aspectRatio, m_CameraComponent->GetZoomLevel());
    }

    void PerspectiveCameraControllerComponent::OnUpdate(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        const float moveSpeed = m_BaseMoveSpeed * a_DeltaTime;
        const float rotateSpeed = m_BaseMoveSpeed * a_DeltaTime;

        //Move

        if (Input::GetKeyDown(MUSE_KEY_A))
        {
            GetTransform()->Move(glm::vec2(-moveSpeed, 0.0f));
        }
        else if (Input::GetKeyDown(MUSE_KEY_D))
        {
            GetTransform()->Move(glm::vec2(moveSpeed, 0.0f));
        }

        if (Input::GetKeyDown(MUSE_KEY_S))
        {
            GetTransform()->Move(glm::vec3(0.0f, 0.0f, -moveSpeed));
        }
        else if (Input::GetKeyDown(MUSE_KEY_W))
        {
            GetTransform()->Move(glm::vec3(0.0f, 0.0f, moveSpeed));
        }

        if (Input::GetKeyDown(MUSE_KEY_LEFT_SHIFT))
        {
            GetTransform()->Move(glm::vec2(0.0f, -moveSpeed));
        }
        else if (Input::GetKeyDown(MUSE_KEY_SPACE))
        {
            GetTransform()->Move(glm::vec2(0.0f, moveSpeed));
        }

        //Rotate

        if (Input::GetKeyDown(MUSE_KEY_LEFT))
        {
            GetTransform()->Rotate(glm::vec3(0.0f, -rotateSpeed, 0.0f));
        }
        else if (Input::GetKeyDown(MUSE_KEY_RIGHT))
        {
            GetTransform()->Rotate(glm::vec3(0.0f, rotateSpeed, 0.0f));
        }

        if (Input::GetKeyDown(MUSE_KEY_DOWN))
        {
            GetTransform()->Rotate(glm::vec3(-rotateSpeed, 0.0f, 0.0f));
        }
        else if (Input::GetKeyDown(MUSE_KEY_UP))
        {
            GetTransform()->Rotate(glm::vec3(rotateSpeed, 0.0f, 0.0f));
        }
    }

    void PerspectiveCameraControllerComponent::OnMouseScrolledEvent(float a_XOffset, float a_YOffset) const
    {
        MUSE_PROFILE_FUNCTION();

        float zoomLevel = m_CameraComponent->GetZoomLevel() - (a_YOffset * m_ZoomSpeed);
        zoomLevel = std::max(zoomLevel, m_MinZoomLevel);
        m_CameraComponent->SetProjection(m_CameraComponent->GetAspectRatio(), zoomLevel);
    }

    void PerspectiveCameraControllerComponent::OnWindowResizeEvent(int a_Width, int a_Height) const
    {
        MUSE_PROFILE_FUNCTION();

        const float aspectRatio = static_cast<float>(a_Width) / static_cast<float>(a_Height);
        m_CameraComponent->SetProjection(aspectRatio, m_CameraComponent->GetZoomLevel());
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::PerspectiveCameraControllerComponent>("PerspectiveCameraControllerComponent")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        );
//.property("Enabled", &Muse::Component::m_isEnabled);
}
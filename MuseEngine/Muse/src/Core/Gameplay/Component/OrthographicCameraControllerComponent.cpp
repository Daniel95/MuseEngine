#include "MusePCH.h"
#include "OrthographicCameraControllerComponent.h"
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
    OrthographicCameraControllerComponent::OrthographicCameraControllerComponent()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().GetWindow().MouseScrolledEvent.Subscribe(SUB_FN(OrthographicCameraControllerComponent::OnMouseScrolledEvent, std::placeholders::_1, std::placeholders::_2));
        Application::Get().GetWindow().WindowResizeEvent.Subscribe(SUB_FN(OrthographicCameraControllerComponent::OnWindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
    }

    OrthographicCameraControllerComponent::~OrthographicCameraControllerComponent()
    {
        MUSE_PROFILE_FUNCTION();

        Application::Get().GetWindow().MouseScrolledEvent.Unsubscribe(this);
        Application::Get().GetWindow().WindowResizeEvent.Unsubscribe(this);
    }

    void OrthographicCameraControllerComponent::OnInit()
    {
        MUSE_PROFILE_FUNCTION();

        m_CameraComponent = GetGameObject()->GetComponent<CameraComponent>();

        ASSERT_ENGINE(m_CameraComponent != nullptr, "CameraComponent on OrthographicCameraControllerComponent GameObject is missing!");

        const float width = static_cast<float>(Application::Get().GetWindow().GetWidth());
        const float height = static_cast<float>(Application::Get().GetWindow().GetHeight());
        const float aspectRatio = width / height;

        m_CameraComponent->SetProjection(aspectRatio, m_CameraComponent->GetZoomLevel());
    }

    void OrthographicCameraControllerComponent::OnUpdate(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

        const float moveSpeed = m_BaseMoveSpeed * a_DeltaTime * m_CameraComponent->GetZoomLevel();

        if (Input::GetKeyDown(MUSE_KEY_A))
        {
            GetTransform()->Translate(glm::vec2(-moveSpeed, 0.0f));
        }
        else if (Input::GetKeyDown(MUSE_KEY_D))
        {
            GetTransform()->Translate(glm::vec2(moveSpeed, 0.0f));
        }

        if (Input::GetKeyDown(MUSE_KEY_S))
        {
            GetTransform()->Translate(glm::vec2(0.0f, -moveSpeed));
        }
        else if (Input::GetKeyDown(MUSE_KEY_W))
        {
            GetTransform()->Translate(glm::vec2(0.0f, moveSpeed));
        }
    }

    void OrthographicCameraControllerComponent::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        MUSE_PROFILE_FUNCTION();

        float zoomLevel = m_CameraComponent->GetZoomLevel() - (a_YOffset * m_ZoomSpeed);
        zoomLevel = std::max(zoomLevel, m_MinZoomLevel);
        m_CameraComponent->SetProjection(m_CameraComponent->GetAspectRatio(), zoomLevel);
    }

    void OrthographicCameraControllerComponent::OnWindowResizeEvent(int a_Width, int a_Height)
    {
        MUSE_PROFILE_FUNCTION();

        const float aspectRatio = static_cast<float>(a_Width) / static_cast<float>(a_Height);
        m_CameraComponent->SetProjection(aspectRatio, m_CameraComponent->GetZoomLevel());
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::OrthographicCameraControllerComponent>("OrthographicCameraControllerComponent")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        );
//.property("Enabled", &Muse::Component::m_isEnabled);
}
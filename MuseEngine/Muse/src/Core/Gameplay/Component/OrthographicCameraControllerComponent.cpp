#include "MusePCH.h"
#include "OrthographicCameraControllerComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "TransformComponent.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "CameraComponent.h"
#include "Core/Gameplay/GameObject.h"

namespace Muse
{
    OrthographicCameraControllerComponent::OrthographicCameraControllerComponent()
    {
        Application::Get().GetWindow().MouseScrolledEvent.Subscribe(this, std::bind(&OrthographicCameraControllerComponent::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        Application::Get().GetWindow().WindowResizeEvent.Subscribe(this, std::bind(&OrthographicCameraControllerComponent::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
    }

    OrthographicCameraControllerComponent::~OrthographicCameraControllerComponent()
    {
        Application::Get().GetWindow().MouseScrolledEvent.Unsubscribe(this);
        Application::Get().GetWindow().WindowResizeEvent.Unsubscribe(this);
    }

    void OrthographicCameraControllerComponent::OnInit()
    {
        m_CameraComponent = GetGameObject()->GetComponent<CameraComponent>();

        ASSERT_ENGINE(m_CameraComponent != nullptr, "CameraComponent on OrthographicCameraControllerComponent GameObject is missing!");

        const float width = static_cast<float>(Application::Get().GetWindow().GetWidth());
        const float height = static_cast<float>(Application::Get().GetWindow().GetHeight());
        const float aspectRatio = width / height;

        m_CameraComponent->SetProjection(aspectRatio, m_CameraComponent->GetZoomLevel());
    }

    void OrthographicCameraControllerComponent::OnUpdate(float a_DeltaTime)
    {
        if (Input::IsKeyPressed(MUSE_KEY_A))
        {
            GetTransform()->Move(glm::vec2(-m_MoveSpeed * a_DeltaTime, 0.0f));
        }
        else if (Input::IsKeyPressed(MUSE_KEY_D))
        {
            GetTransform()->Move(glm::vec2(m_MoveSpeed * a_DeltaTime, 0.0f));
        }

        if (Input::IsKeyPressed(MUSE_KEY_S))
        {
            GetTransform()->Move(glm::vec2(0.0f, -m_MoveSpeed * a_DeltaTime));
        }
        else if (Input::IsKeyPressed(MUSE_KEY_W))
        {
            GetTransform()->Move(glm::vec2(0.0f, m_MoveSpeed * a_DeltaTime));
        }
    }

    void OrthographicCameraControllerComponent::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        float zoomLevel = m_CameraComponent->GetZoomLevel() - (a_YOffset * m_ZoomSpeed);
        m_CameraComponent->SetProjection(m_CameraComponent->GetAspectRatio(), zoomLevel);
    }

    void OrthographicCameraControllerComponent::OnWindowResizeEvent(int a_Width, int a_Height)
    {
        const float aspectRatio = static_cast<float>(a_Width) / static_cast<float>(a_Height);
        m_CameraComponent->SetProjection(aspectRatio, m_CameraComponent->GetZoomLevel());
    }
}

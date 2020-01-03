#include "MusePCH.h"

#include "CameraComponent.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponent.h"

namespace Muse
{
    CameraComponent* CameraComponent::s_MainCamera = nullptr;

    CameraComponent::CameraComponent()
        : Component()
    {
        s_MainCamera = this;
    }

    void CameraComponent::SetProjection(float a_AspectRatio, float a_ZoomLevel)
    {
        m_AspectRatio = a_AspectRatio;
        m_ZoomLevel = a_ZoomLevel;
        SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    const glm::mat4& CameraComponent::GetViewMatrix() const
    {
        return GetGameObject()->GetTransform()->GetModelMatrix();
    }

    void CameraComponent::RecalculateViewMatrix()
    {
        m_ViewMatrix = glm::inverse(GetViewMatrix());
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void CameraComponent::SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near, float a_Far)
    {
        m_ProjectionMatrix = glm::ortho(a_Left, a_Right, a_Bottom, a_Top, a_Near, a_Far);
    }
}

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
        SetProjectionMatrix(-1.6f, 1.6f, -0.9f, 0.9f);
        s_MainCamera = this;
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

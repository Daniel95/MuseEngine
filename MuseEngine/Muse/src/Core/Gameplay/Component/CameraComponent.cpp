#include "MusePCH.h"

#include "CameraComponent.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    CameraComponent* CameraComponent::s_MainCamera = nullptr;

    CameraComponent::CameraComponent()
        : Component()
    {
        MUSE_PROFILE_FUNCTION();

        s_MainCamera = this;
    }

    void CameraComponent::SetProjection(float a_AspectRatio, float a_ZoomLevel)
    {
        MUSE_PROFILE_FUNCTION();

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
        MUSE_PROFILE_FUNCTION();

        m_ViewMatrix = glm::inverse(GetViewMatrix());
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void CameraComponent::SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near, float a_Far)
    {
        MUSE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(a_Left, a_Right, a_Bottom, a_Top, a_Near, a_Far);
    }

    void CameraComponent::SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix)
    {
        MUSE_PROFILE_FUNCTION();

        m_ProjectionMatrix = a_ProjectionMatrix;
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::CameraComponent>("CameraComponent")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        )
        .property("m_AspectRatio", &Muse::CameraComponent::m_AspectRatio)
        .property("m_ProjectionMatrix", &Muse::CameraComponent::m_ProjectionMatrix)
        .property("m_ViewProjectionMatrix", &Muse::CameraComponent::m_ViewProjectionMatrix)
        .property("m_IsEditorCamera", &Muse::CameraComponent::m_IsEditorCamera)
        .property("m_ZoomLevel", &Muse::CameraComponent::m_ZoomLevel)
        .property("m_AspectRatio", &Muse::CameraComponent::m_AspectRatio);
}
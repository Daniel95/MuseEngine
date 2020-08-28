#include "MusePCH.h"

#include "CameraComponentOld.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponentOld.h"
#include "Core/Instrumentor.h"

namespace Muse
{
    CameraComponentOld* CameraComponentOld::s_MainCamera = nullptr;

    CameraComponentOld::CameraComponentOld()
        : Component()
    {
        MUSE_PROFILE_FUNCTION();
    }

    void CameraComponentOld::OnInit()
    {
        Component::OnInit();

        s_MainCamera = this;
    }

    void CameraComponentOld::SetProjection(float a_AspectRatio, float a_ZoomLevel)
    {
        MUSE_PROFILE_FUNCTION();

        m_AspectRatio = a_AspectRatio;
        m_ZoomLevel = a_ZoomLevel;
        SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    const glm::mat4& CameraComponentOld::GetViewMatrix() const
    {
        return GetGameObject()->GetTransform()->GetWorldModelMatrix();
    }

    void CameraComponentOld::RecalculateViewMatrix()
    {
        MUSE_PROFILE_FUNCTION();

        m_ViewMatrix = glm::inverse(GetViewMatrix());
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void CameraComponentOld::SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near, float a_Far)
    {
        MUSE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(a_Left, a_Right, a_Bottom, a_Top, a_Near, a_Far);
    }

    void CameraComponentOld::SetProjectionMatrix(const glm::mat4& a_ProjectionMatrix)
    {
        MUSE_PROFILE_FUNCTION();

        m_ProjectionMatrix = a_ProjectionMatrix;
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::CameraComponentOld>("CameraComponentOld")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        )
        .property("m_AspectRatio", &Muse::CameraComponentOld::m_AspectRatio)
        .property("m_ProjectionMatrix", &Muse::CameraComponentOld::m_ProjectionMatrix)
        .property("m_ViewProjectionMatrix", &Muse::CameraComponentOld::m_ViewProjectionMatrix)
        .property("m_IsEditorCamera", &Muse::CameraComponentOld::m_IsEditorCamera)
        .property("m_ZoomLevel", &Muse::CameraComponentOld::m_ZoomLevel)
        .property("m_AspectRatio", &Muse::CameraComponentOld::m_AspectRatio);
}
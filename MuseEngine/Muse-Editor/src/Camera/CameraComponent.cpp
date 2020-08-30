#include "CameraComponent.h"
#include "Core/Instrumentor.h"
#include "Core/ECS/Component/TransformComponent.h"

namespace Muse
{
    const glm::mat4& CameraComponent::GetProjectionViewMatrix(TransformComponent& a_TransformComponent)
    {
        if (m_IsDirty || a_TransformComponent.GetDirty())
        {
            m_ViewMatrix = glm::inverse(a_TransformComponent.GetWorldModelMatrix());
            m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
            m_IsDirty = false;
        }

        return m_ViewProjectionMatrix;
    }

    void CameraComponent::SetProjection(float a_AspectRatio, float a_ZoomLevel)
    {
        MUSE_PROFILE_FUNCTION();

        m_IsDirty = true;

        m_AspectRatio = a_AspectRatio;
        m_ZoomLevel = a_ZoomLevel;
        SetProjectionMatrix(-a_AspectRatio * a_ZoomLevel, a_AspectRatio * a_ZoomLevel, -a_ZoomLevel, a_ZoomLevel);
    }

    void CameraComponent::SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near, float a_Far)
    {
        MUSE_PROFILE_FUNCTION();

        m_IsDirty = true;

        m_ProjectionMatrix = glm::ortho(a_Left, a_Right, a_Bottom, a_Top, a_Near, a_Far);
    }
}


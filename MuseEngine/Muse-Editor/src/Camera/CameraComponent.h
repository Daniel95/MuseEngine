#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Muse
{
    struct TransformComponent;

    class CameraComponent
    {
    public:
        void SetProjection(float a_AspectRatio, float a_ZoomLevel);
        void SetProjectionMatrix(float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0, float a_Far = 1.0);
        const glm::mat4& GetProjectionViewMatrix(TransformComponent& a_TransformComponent);

        void SetZoomLevel(float a_ZoomLevel) { SetProjection(m_AspectRatio, a_ZoomLevel); }
        float GetZoomLevel() { return m_ZoomLevel; }

        void SetAspectRatio(float a_AspectRatio) { SetProjection(a_AspectRatio, m_ZoomLevel); }
        float GetAspectRatio() { return m_AspectRatio; }

    private:
        glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ProjectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_ViewProjectionMatrix = glm::identity<glm::mat4>();
        float m_ZoomLevel = 1.0f;
        float m_AspectRatio = 1.6f;

        bool m_IsDirty = true;

        static CameraComponent* s_MainCamera;
    };
}